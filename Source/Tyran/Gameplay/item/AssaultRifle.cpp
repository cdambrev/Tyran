// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"
#include "ImpactEffect.h"
#include "Camera/CameraComponent.h"

#define COLLISION_WEAPON ECC_GameTraceChannel3

AAssaultRifle::AAssaultRifle()
{
	TimeBetweenShots = 0.1f;
	HitDamage = 26; 
	WeaponRange = 10000; 
	AllowedViewDotHitDir = 0.8f; 
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800; 
	TracerRoundInterval = 3;
	Accuracy = 20.0f;
	MagazineSize = 30;
	MagazineCurrent = 30;

	StorageSlot = EInventorySlot::Primary; 
	AmmoType = EAmmoType::AssaultRifle;
	RifleAttachPoint = TEXT("RifleSocket"); 

	//GetWeaponMesh()->AddLocalRotation(FRotator(0, 0, -90)); 
}

void AAssaultRifle::FireWeapon()
{
	UpdateSpreadVector();
	const FVector AimDir = GetAdjustedAim(); // Obtenir le «ciblage» 
	//const FVector StartPos = GetCameraDamageStartLocation(AimDir); // Position de départ du tir 
	const FVector StartPos = GetMuzzleLocation();
	const FVector EndPos = StartPos + (AimDir * WeaponRange); // Position de fin du tir 
	
	const FHitResult Impact = WeaponTrace(StartPos, EndPos); // Trouver l'impact 
	ProcessInstantHit(Impact, StartPos, AimDir); // Traiter l'impact
	//MagazineCurrent--;
}

FVector AAssaultRifle::GetAdjustedAim() const
{
	APlayerController* const PC = Instigator ? Cast<APlayerController>(Instigator->Controller) : nullptr; 
	FVector FinalAim = FVector::ZeroVector; 
	
	if (PC) { 
		FVector CamLoc; 
		FRotator CamRot; 
		PC->GetPlayerViewPoint(CamLoc, CamRot); 
		FinalAim = CamRot.Vector(); 
	} else if (Instigator) { 
		FinalAim = Instigator->GetBaseAimRotation().Vector(); 
	} 

	FinalAim += SpreadVector;
	FinalAim.Normalize();
	
	return FinalAim;
}

FVector AAssaultRifle::GetCameraDamageStartLocation(const FVector & AimDir) const
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

FHitResult AAssaultRifle::WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator); 
	TraceParams.bTraceAsyncScene = true; 
	TraceParams.bReturnPhysicalMaterial = true; 
	FHitResult Hit(ForceInit); 
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams); 
	
	return Hit;
}

void AAssaultRifle::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir)
{
	if (MyPawn && MyPawn->IsLocallyControlled() && GetNetMode() == NM_Client) { 
		// Si nous sommes client et frappons quelque chose contrôlé par le serveur 
		if (Impact.GetActor() && Impact.GetActor()->GetRemoteRole() == ROLE_Authority) { 
			// Avertir le serveur de notre frappe pour valider et appliquer le dommage. 
			ServerNotifyHit(Impact, ShootDir); 
		} else if (Impact.GetActor() == nullptr) { 
			if (Impact.bBlockingHit) {
				ServerNotifyHit(Impact, ShootDir);
			} else { 
				ServerNotifyMiss(ShootDir); 
			}
		}
	} 
	// Traiter une frappe confirmée. 
	ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
}

bool AAssaultRifle::ServerNotifyHit_Validate(const FHitResult Impact, /*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	return true; 
}

void AAssaultRifle::ServerNotifyHit_Implementation(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir) {
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
			} else { 
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

bool AAssaultRifle::ServerNotifyMiss_Validate(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	return true; 
} 

void AAssaultRifle::ServerNotifyMiss_Implementation(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir) {
	const FVector Origin = GetMuzzleLocation();

	// Sur les clients distants 
	HitOriginNotify = Origin; 
	const FVector EndTrace = Origin + (ShootDir * WeaponRange); 

	if (GetNetMode() != NM_DedicatedServer) { 
		SpawnTrailEffects(EndTrace); 
	}
}

void AAssaultRifle::ProcessInstantHitConfirmed(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir)
{
	// Traiter les dommages 
	if (ShouldDealDamage(Impact.GetActor())) { 
		DealDamage(Impact, ShootDir); 
	} 
	
	// Jouer l'effet visuel sur les clients distants 
	if (Role == ROLE_Authority) { 
		HitOriginNotify = Origin; 
	} 
	
	// Jouer l'effet visuel localement 
	if (GetNetMode() != NM_DedicatedServer) { 
		SimulateInstantHit(Origin); 
	}
}

bool AAssaultRifle::ShouldDealDamage(AActor * TestActor) const
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

void AAssaultRifle::DealDamage(const FHitResult & Impact, const FVector & ShootDir)
{
	FPointDamageEvent PointDmg;
	PointDmg.DamageTypeClass = DamageType;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = HitDamage;

	Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
}

void AAssaultRifle::SimulateInstantHit(const FVector & Origin)
{
	const FVector StartTrace = Origin; 
	const FVector AimDir = GetAdjustedAim(); 
	const FVector EndTrace = StartTrace + (AimDir * WeaponRange); 
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace); 
	if (Impact.bBlockingHit) { 
		SpawnImpactEffects(Impact); 
		SpawnTrailEffects(Impact.ImpactPoint); 
	} else { 
		SpawnTrailEffects(EndTrace); 
	}
}

void AAssaultRifle::SpawnImpactEffects(const FHitResult & Impact)
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

void AAssaultRifle::SpawnTrailEffects(const FVector & EndPoint)
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
			UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, Origin, ShootDir.Rotation()); 
		} 
	} else { 
		// Ignorer les trainées lorsqu'elles sont créées par nous. 
		ATyranCharacter* OwningPawn = MyPawn; 
		if (OwningPawn && OwningPawn->IsLocallyControlled())
			return;
		
		if (TrailFX) { 
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin); 
			if (TrailPSC) { 
				TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint); 
			}
		}
	}
}

void AAssaultRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME_CONDITION(AAssaultRifle, HitOriginNotify, COND_SkipOwner);
}

void AAssaultRifle::OnRep_HitLocation()
{
	// À jouer sur tous les clients distants
	SimulateInstantHit(HitOriginNotify);
}

void AAssaultRifle::UpdateSpreadVector()
{
	// Dispersion
	float SpreadFactor = 1.0f/Accuracy;
	if (MyPawn->isAiming)
		SpreadFactor /= 8;

	float x = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float y = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float z = FMath::RandRange(-SpreadFactor, SpreadFactor);
	SpreadVector = FVector{x,y,z};
}
