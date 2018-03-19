// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "ImpactEffect.h"

#define COLLISION_WEAPON ECC_GameTraceChannel3

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetSimulatePhysics(false);
	RootComponent = Mesh;

	StorageSlot = EInventorySlot::Primary;

	bIsEquipped = false;
	bPendingEquip = false;

	// Pour multi-joueurs 
	bReplicates = true;
	bReplicateMovement = true;
	bNetUseOwnerRelevancy = true;

	MuzzleAttachPoint = TEXT("MuzzleFlashSocket");
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::OnEnterInventory(ATyranCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
	AttachMeshToPawn(StorageSlot);
}

void AWeapon::OnLeaveInventory()
{
	if (Role == ROLE_Authority) {
		SetOwningPawn(nullptr);
	}

	if (IsEquippedOrPending()) {
		OnUnEquip();
	}
	DetachMeshFromPawn();

}

bool AWeapon::IsEquippedOrPending() const
{
	return bIsEquipped || bPendingEquip;
}

void AWeapon::OnUnEquip()
{
	AttachMeshToPawn(StorageSlot);
	bIsEquipped = false;
	// StopFire(); 

	if (bPendingEquip) {
		StopWeaponAnimation(EquipAnim);
		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(EquipFinishedTimerHandle);
	}

	if (bPendingReload) {
		StopWeaponAnimation(ReloadAnim);
		bPendingReload = false;

		GetWorldTimerManager().ClearTimer(ReloadFinishedTimerHandle);
	}

	DetermineWeaponState();
}

void AWeapon::OnEquip()
{
	AttachMeshToPawn();

	bPendingEquip = true;
	DetermineWeaponState();

	float Duration = PlayWeaponAnimation(EquipAnim);
	if (Duration <= 0.0f) {
		// Pour ne pas avoir de problème 
		Duration = 0.5f;
	}
	EquipStartedTime = GetWorld()->TimeSeconds;
	EquipDuration = Duration;

	GetWorldTimerManager().SetTimer(EquipFinishedTimerHandle, this, &AWeapon::OnEquipFinished, Duration, false);
	/*if (MyPawn && MyPawn->IsLocallyControlled()) {
		PlayWeaponSound(EquipSound);
	}*/
}

void AWeapon::DetermineWeaponState()
{
	EWeaponState NewState = EWeaponState::Idle;
	if (bIsEquipped) {
		if (bWantsToFire && CanFire()) 
		{ 
			NewState = EWeaponState::Firing;
		} 
	}
	else if (bPendingEquip) {
		NewState = EWeaponState::Equipping;
	}

	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState NewState)
{
	const EWeaponState PrevState = CurrentState;

	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing) {
		OnBurstFinished(); 
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing) {
		OnBurstStarted(); 
	}
}

EWeaponState AWeapon::GetCurrentState() const
{
	return CurrentState;
}

bool AWeapon::IsEquipped() const
{
	return bIsEquipped;
}

void AWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;
	bPendingEquip = false;

	DetermineWeaponState();
}

float AWeapon::GetEquipStartedTime() const
{
	return EquipStartedTime;
}

float AWeapon::GetEquipDuration() const
{
	return EquipDuration;
}

float AWeapon::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	float Duration = 0.0f;
	if (MyPawn) {
		Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
	}

	return Duration;
}

void AWeapon::StopWeaponAnimation(UAnimMontage * Animation)
{
	if (MyPawn) {
		if (Animation) {
			MyPawn->StopAnimMontage(Animation);
		}
	}
}

void AWeapon::StartFire()
{
	if (Role < ROLE_Authority) {
		ServerStartFire();
	}
	if (!bWantsToFire) {
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AWeapon::StopFire() {
	if (Role < ROLE_Authority) {
		ServerStopFire();
	}
	if (bWantsToFire) {
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

bool AWeapon::ServerStartFire_Validate() {
	return true;
}

void AWeapon::ServerStartFire_Implementation() {
	StartFire();
}

bool AWeapon::ServerStopFire_Validate() {
	return true;
}

void AWeapon::ServerStopFire_Implementation() {
	StopFire();
}

void AWeapon::OnBurstStarted() {
	// Démarrer le tir, pourra être retardé pour tenir compte de TimeBetweenShots 
	const float GameTime = GetWorld()->GetTimeSeconds(); 

	if (LastFireTime > 0 && TimeBetweenShots > 0.0f && LastFireTime + TimeBetweenShots > GameTime) { 
		GetWorldTimerManager().SetTimer(HandleFiringTimerHandle, this, &AWeapon::HandleFiring, LastFireTime + TimeBetweenShots - GameTime, false); 
	} else { 
		HandleFiring(); 
	} 
} 

void AWeapon::OnBurstFinished() { 
	BurstCounter = 0; 

	if (GetNetMode() != NM_DedicatedServer) { 
		StopSimulatingWeaponFire(); 
	} 

	GetWorldTimerManager().ClearTimer(HandleFiringTimerHandle); 
	bRefiring = false; 
}

void AWeapon::HandleFiring() {
	if (CanFire()) {
		if (Role == ROLE_Authority) {
			SimulateWeaponFire();
		}
		else
		{
			SimulateWeaponFireServer();
		}
		if (MyPawn && MyPawn->IsLocallyControlled()) {
			FireWeapon(); 

			// Mettre à jour l'effet sur les objets distants 
			BurstCounter++; 
		} 
		MagazineCurrent--;
	}
	else if (MagazineCurrent == 0)
	{
		OnReload();
	}
	if (MyPawn && MyPawn->IsLocallyControlled()) { 
		if (Role < ROLE_Authority) { 
			ServerHandleFiring(); 
		} 
		
		bRefiring = (CurrentState == EWeaponState::Firing && TimeBetweenShots > 0.0f); 
		if (bRefiring) { 
			GetWorldTimerManager().SetTimer(HandleFiringTimerHandle, this, &AWeapon::HandleFiring, TimeBetweenShots, false); 
		} 
	} 
	
	LastFireTime = GetWorld()->GetTimeSeconds(); 
}

bool AWeapon::ServerHandleFiring_Validate() { 
	return true; 
} 

void AWeapon::ServerHandleFiring_Implementation() {
	const bool bShouldUpdateAmmo = CanFire(); 
	HandleFiring(); 
	if (bShouldUpdateAmmo) { 
		// Mettre à jour l'effet sur les clients distants 
		BurstCounter++; 
	} 
}

bool AWeapon::CanFire() const { 
	bool bPawnCanFire = MyPawn && MyPawn->CanFire(); 
	bool bStateOK = CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing; 
	bool bNotMagEmpty = MagazineCurrent > 0;
	return bPawnCanFire && bStateOK && bNotMagEmpty; 
}

void AWeapon::SimulateWeaponFire() { 
	SpawnMuzzleEffectsMulticast();
	
	//if (!bPlayingFireAnim) { 
	//	PlayWeaponAnimation(FireAnim); 
	//	bPlayingFireAnim = true; 
	//} 
	//PlayWeaponSound(FireSound); 
}

bool AWeapon::SimulateWeaponFireServer_Validate() {
	return true;
}

void AWeapon::SimulateWeaponFireServer_Implementation() {
	SimulateWeaponFire();
}

bool AWeapon::SpawnMuzzleEffectsMulticast_Validate() {
	return true;
}

void AWeapon::SpawnMuzzleEffectsMulticast_Implementation() {
	if (MuzzleFX) {
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, MuzzleAttachPoint);

		// TODO: Trouver une meilleure solution
		if (WeaponType == EWeaponType::Pistol)
		{
			MuzzlePSC->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		}
	}
}


void AWeapon::StopSimulatingWeaponFire() {
	/*if (bPlayingFireAnim) {
		StopWeaponAnimation(FireAnim);
		bPlayingFireAnim = false;
	}*/
}

FVector AWeapon::GetMuzzleLocation() const {
	return Mesh->GetSocketLocation(MuzzleAttachPoint); 
} 

FVector AWeapon::GetMuzzleDirection() const {
	return Mesh->GetSocketRotation(MuzzleAttachPoint).Vector();
}

EAmmoType AWeapon::GetAmmoType()
{
	return AmmoType;
}

EWeaponType AWeapon::GetWeaponType()
{
	return WeaponType;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetOwningPawn(ATyranCharacter * NewOwner)
{
	if (MyPawn != NewOwner) {
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// Propriétaire réseau pour les appels RPC. 
		SetOwner(NewOwner);
	}
}

void AWeapon::AttachMeshToPawn(EInventorySlot Slot)
{
	if (MyPawn) {
		// Retirer et cacher 
		DetachMeshFromPawn();

		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = MyPawn->GetInventoryAttachPoint(Slot, WeaponType);
		Mesh->SetHiddenInGame(false);
		// On le rend visible
		Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

void AWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetHiddenInGame(true);
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
{
	return Mesh;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, MyPawn);
	DOREPLIFETIME(AWeapon, MagazineCurrent);
	DOREPLIFETIME(AWeapon, bPendingReload);
}

void AWeapon::OnRep_MyPawn() {
	if (MyPawn) {
		OnEnterInventory(MyPawn);
	}
	else {
		OnLeaveInventory();
	}
}

int AWeapon::getMagCurrent()
{
	return MagazineCurrent;
}

void AWeapon::OnReload()
{
	if (MyPawn->Ammunition[AmmoType] > 0 && MagazineCurrent < MagazineSize && !bPendingReload)
	{
		bPendingReload = true;

		float Duration = PlayWeaponAnimation(ReloadAnim);
		if (Duration <= 0.0f) {
			// Pour ne pas avoir de problème 
			Duration = 0.5f;
		}
		ReloadStartedTime = GetWorld()->TimeSeconds;
		ReloadDuration = Duration;

		GetWorldTimerManager().SetTimer(ReloadFinishedTimerHandle, this, &AWeapon::OnReloadFinished, Duration, false);
	}
}

bool AWeapon::ServerReload_Validate() {
	return true;
}

void AWeapon::ServerReload_Implementation() {
	OnReload();
}

void AWeapon::OnReloadFinished()
{
	bPendingReload = false;

	int NbAmmoToFill = MagazineSize - MagazineCurrent;
	if (MyPawn->Ammunition[AmmoType] > NbAmmoToFill)
	{
		MagazineCurrent = MagazineSize;
		MyPawn->Ammunition[AmmoType] -= NbAmmoToFill;
	} 
	else
	{
		MagazineCurrent += MyPawn->Ammunition[AmmoType];
		MyPawn->Ammunition[AmmoType] = 0;
	}

	DetermineWeaponState();
}

void AWeapon::OnRep_BurstCounter() {
	if (BurstCounter > 0) { 
		SimulateWeaponFire(); 
	} else { 
		StopSimulatingWeaponFire(); 
	} 
}

void AWeapon::OnRep_bPendingReload()
{
	if (bPendingReload)
	{
		PlayWeaponAnimation(ReloadAnim);
		//OnReload();
	}
	/*else
	{
		OnReloadFinished();
	}*/
}

FVector AWeapon::GetAdjustedAim() const
{
	APlayerController* const PC = Instigator ? Cast<APlayerController>(Instigator->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;

	if (PC) {
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if (Instigator) {
		FinalAim = Instigator->GetBaseAimRotation().Vector();
	}

	FinalAim += SpreadVector;
	FinalAim.Normalize();

	return FinalAim;
}

FVector AWeapon::GetCameraDamageStartLocation(const FVector & AimDir) const
{
	APlayerController* PC = MyPawn ? Cast<APlayerController>(MyPawn->Controller) : nullptr;
	FVector OutStartTrace = FVector::ZeroVector;

	if (PC) {
		FRotator dummyRot;
		PC->GetPlayerViewPoint(OutStartTrace, dummyRot);

		// Ajuster la trace pour qu'il n'y ait rien qui bloque le rayon entre la caméra et le
		// personnage, on calcule ensuite la distance à partir du point de départ ajusté. 
		OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace) | AimDir);
	}

	return OutStartTrace;
}

FHitResult AWeapon::WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir)
{
	if (MyPawn && MyPawn->IsLocallyControlled() && GetNetMode() == NM_Client) {
		// Si nous sommes client et frappons quelque chose contrôlé par le serveur 
		if (Impact.GetActor() && Impact.GetActor()->GetRemoteRole() == ROLE_Authority) {
			// Avertir le serveur de notre frappe pour valider et appliquer le dommage. 
			ServerNotifyHit(Impact, ShootDir);
		}
		else if (Impact.GetActor() == nullptr) {
			if (Impact.bBlockingHit) {
				ServerNotifyHit(Impact, ShootDir);
			}
			else {
				ServerNotifyMiss(ShootDir);
			}
		}
	}
	// Traiter une frappe confirmée. 
	//ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
}

bool AWeapon::ServerNotifyHit_Validate(const FHitResult Impact, /*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	return true;
}

void AWeapon::ServerNotifyHit_Implementation(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir) {
	// Si nous avons un instigateur, nous calculons le produit vectoriel entre la vue et le tir. 
	if (Instigator && (Impact.GetActor() || Impact.bBlockingHit)) {
		const FVector Origin = GetMuzzleLocation();
		const FVector ViewDir = (Impact.Location - Origin).GetSafeNormal();

		const float ViewDotHitDir = FVector::DotProduct(Instigator->GetViewRotation().Vector(), ViewDir);
		if (ViewDotHitDir > AllowedViewDotHitDir) {
			if (Impact.GetActor() == nullptr) {
				if (Impact.bBlockingHit) {
					ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
				}
			}
			// On espère que les trucs statiques soient OK mais de toute façon, ils 
			// ont peu ou pas d'impacts sur le gameplay 
			else if (Impact.GetActor()->IsRootComponentStatic() || Impact.GetActor()->IsRootComponentStationary()) {
				ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
			}
			else {
				const FBox HitBox = Impact.GetActor()->GetComponentsBoundingBox();
				FVector BoxExtent = 0.5 * (HitBox.Max - HitBox.Min);
				BoxExtent *= ClientSideHitLeeway;
				BoxExtent.X = FMath::Max(20.0f, BoxExtent.X);
				BoxExtent.Y = FMath::Max(20.0f, BoxExtent.Y);
				BoxExtent.Z = FMath::Max(20.0f, BoxExtent.Z);
				const FVector BoxCenter = (HitBox.Min + HitBox.Max) * 0.5;

				// Si nous sommes dans la zone d'impact 
				if (FMath::Abs(Impact.Location.Z - BoxCenter.Z) < BoxExtent.Z &&
					FMath::Abs(Impact.Location.X - BoxCenter.X) < BoxExtent.X &&
					FMath::Abs(Impact.Location.Y - BoxCenter.Y) < BoxExtent.Y) {
					ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
				}
			}
		}
	}
}

bool AWeapon::ServerNotifyMiss_Validate(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	return true;
}

void AWeapon::ServerNotifyMiss_Implementation(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	const FVector Origin = GetMuzzleLocation();

	// Sur les clients distants 
	//HitOriginNotify = Origin; 
	const FVector EndTrace = Origin + (ShootDir * WeaponRange);

	if (GetNetMode() != NM_DedicatedServer) {
		SpawnTrailEffectsMulticast(EndTrace);
	}
}

void AWeapon::ProcessInstantHitConfirmed(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir)
{
	// Traiter les dommages 
	if (ShouldDealDamage(Impact.GetActor())) {
		DealDamage(Impact, ShootDir);
	}

	// Jouer l'effet visuel sur les clients distants 
	if (Role == ROLE_Authority) {
		//HitOriginNotify = Origin;
		SimulateInstantHit(Origin);
	}
	else {
		SimulateInstantHitServer(Origin);
	}
}

bool AWeapon::ShouldDealDamage(AActor * TestActor) const
{
	// Si nous sommes sur le serveur ou que le client local a priorité sur le serveur, 
	// Nous devons enregistrer le dommage 
	if (TestActor) {
		if (GetNetMode() != NM_Client || TestActor->Role == ROLE_Authority || TestActor->bTearOff) {
			return true;
		}
	}
	return false;
}

void AWeapon::DealDamage(const FHitResult & Impact, const FVector & ShootDir)
{
	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = DamageType;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = HitDamage;

	Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
}

void AWeapon::SimulateInstantHit(const FVector & Origin)
{
	const FVector StartTrace = Origin;
	const FVector AimDir = GetAdjustedAim();
	const FVector EndTrace = StartTrace + (AimDir * WeaponRange);
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	if (Impact.bBlockingHit) {
		SpawnImpactEffectsMulticast(Impact);
		SpawnTrailEffectsMulticast(Impact.ImpactPoint);
	}
	else {
		SpawnTrailEffectsMulticast(EndTrace);
	}
}

bool AWeapon::SimulateInstantHitServer_Validate(const FVector & Origin) {
	return true;
}

void AWeapon::SimulateInstantHitServer_Implementation(const FVector & Origin) {
	SimulateInstantHit(Origin);
}


bool AWeapon::SpawnImpactEffectsMulticast_Validate(const FHitResult & Impact) {
	return true;
}


void AWeapon::SpawnImpactEffectsMulticast_Implementation(const FHitResult & Impact)
{
	if (ImpactTemplate && Impact.bBlockingHit) {
		/* Cette prépare un acteur à apparaître (spawn), mais demandera un autre appel pour finir
		le processus de création (d'incarnation?). Nous devons manipuler certaines propriétés
		avant d'entrer dans le niveau */
		AImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<AImpactEffect>
			(ImpactTemplate, FTransform(Impact.ImpactPoint.Rotation(), Impact.ImpactPoint),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (EffectActor) {
			EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		}
	}
}

bool AWeapon::SpawnTrailEffectsMulticast_Validate(const FVector & EndPoint) {
	return true;
}

void AWeapon::SpawnTrailEffectsMulticast_Implementation(const FVector & EndPoint)
{
	// Conserver le compte pour les effets 
	BulletsShotCount++;

	const FVector Origin = GetMuzzleLocation();
	FVector ShootDir = EndPoint - Origin;

	// Faire apparaître l'effet seulement si une distance minimale est respectée. 
	if (ShootDir.Size() < MinimumProjectileSpawnDistance)
		return;

	if (BulletsShotCount % TracerRoundInterval == 0) {
		if (TracerFX) {
			ShootDir.Normalize();
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, Origin, ShootDir.Rotation());
			//emitter->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
		}
	}
	else {
		// Ignorer les trainées lorsqu'elles sont créées par nous. 
		ATyranCharacter* OwningPawn = MyPawn;
		if (OwningPawn && OwningPawn->IsLocallyControlled())
			return;

		if (TrailFX) {
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin);
			if (TrailPSC) {
				TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
				//TrailPSC->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
			}
		}
	}
}

void AWeapon::UpdateSpreadVector()
{
	// Dispersion
	float SpreadFactor = 1.0f / Accuracy;
	if (MyPawn->isAiming)
		SpreadFactor /= 8;

	float x = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float y = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float z = FMath::RandRange(-SpreadFactor, SpreadFactor);
	SpreadVector = FVector{ x,y,z };
}