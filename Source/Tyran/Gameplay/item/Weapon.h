// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Basic/TyranCharacter.h"
#include "Sound/SoundCue.h"
#include "ParticleDefinitions.h"
#include "Weapon.generated.h"

UENUM() enum class EWeaponState {
	Idle, // L'arme ne fait rien 
	Firing, // L'arme fait feu 
	Equipping, // L'arme est en cours d'�quipement (animation) 
	Reloading // L'arme est en recharge (animation) 
};

UCLASS(ABSTRACT, Blueprintable)
class TYRAN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	/* Classe a faire appara�tre lorsque l'objet est lache */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Weapon")
	TSubclassOf<class AWeaponLoot> WeaponLootClass;

protected:
	EWeaponType WeaponType;

	/** Le propri�taire */ 
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ATyranCharacter* MyPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	/* Le socket (emplacement) o� l'objet sera plac� */ 
	EInventorySlot StorageSlot;

	EAmmoType AmmoType;

	// ******* �quipement 
	/** D�part de l'�quipement */ 
	float EquipStartedTime; 
	
	/** Dur�e de l'�quipement */ 
	float EquipDuration; 
	
	bool bIsEquipped;
	
	bool bPendingEquip; 
	
	FTimerHandle EquipFinishedTimerHandle;
		
	// ******* Rechargement 
	/** D�part du rechargement */
	float ReloadStartedTime;

	/** Dur�e du rechargement */
	float ReloadDuration;

	UPROPERTY(ReplicatedUsing = OnRep_bPendingReload)
	bool bPendingReload;

	FTimerHandle ReloadFinishedTimerHandle;

	EWeaponState CurrentState;

	// Animations et son 
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds") 
	USoundCue* EquipSound;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ReloadAnim;

	bool bWantsToFire; 
	bool bRefiring; 
	float LastFireTime; 
	bool bPlayingFireAnim; 
	FTimerHandle HandleFiringTimerHandle; 
	
	UPROPERTY(EditDefaultsOnly) 
	float TimeBetweenShots; 
	UPROPERTY(EditDefaultsOnly, Category = "Sounds") 
	USoundCue* FireSound; 
	UPROPERTY(EditDefaultsOnly) 
	UAnimMontage* FireAnim; 
	UPROPERTY(EditDefaultsOnly) 
	UParticleSystem* MuzzleFX; 
	UPROPERTY(Transient) 
	UParticleSystemComponent* MuzzlePSC; 
	UPROPERTY(EditDefaultsOnly) 
	FName MuzzleAttachPoint; 
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter) 
	int32 BurstCounter;

	UPROPERTY(EditDefaultsOnly)
	int MagazineSize;
	UPROPERTY(Transient, Replicated)
	int MagazineCurrent;

	// *** Variables pour tir et dommages 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly)
	float Accuracy;

	UPROPERTY(EditDefaultsOnly)
	float HitDamage;

	FVector SpreadVector;

	/* Verification d'un �hit�: d�clencheur pour
	le calcul du produit vectoriel entre la
	direction de vision et la direction du hit */
	UPROPERTY(EditDefaultsOnly)
		float AllowedViewDotHitDir;

	/* Verification d'un �hit�: �chelle de
	la boite de l'acteur frapp� */
	UPROPERTY(EditDefaultsOnly)
		float ClientSideHitLeeway;

	//UPROPERTY(Transient, ReplicatedUsing = OnRep_HitLocation) 
	FVector HitOriginNotify;

	/* Effet jou� lorsqu'une surface est atteinte. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly)
	FName TrailTargetParam;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TracerFX;

	/* Distance minimale pour faire appara�tre les traces de balle. */
	UPROPERTY(EditDefaultsOnly)
	float MinimumProjectileSpawnDistance;

	UPROPERTY(EditDefaultsOnly)
	int32 TracerRoundInterval;

	/* Nous comptons les coups */
	int32 BulletsShotCount;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE EInventorySlot GetStorageSlot() { return StorageSlot; }

	/* Prendre en note le propri�taire */ 
	void SetOwningPawn(ATyranCharacter* NewOwner);

	/* Associer l'arme � une position d'inventaire*/
	void AttachMeshToPawn(EInventorySlot Slot = EInventorySlot::Hands);	

	/* Nous d�tachons le mesh de l'acteur - operation 3d */
	void DetachMeshFromPawn();

	virtual void OnEnterInventory(ATyranCharacter* NewOwner);

	virtual void OnLeaveInventory();

	bool IsEquippedOrPending() const;

	virtual void OnUnEquip();

	// �quipement 
	void OnEquip(); 
	
	void DetermineWeaponState(); 
	
	void SetWeaponState(EWeaponState NewState); 
	
	EWeaponState GetCurrentState() const; 
	
	bool IsEquipped() const; 
	
	virtual void OnEquipFinished(); 
	
	float GetEquipStartedTime() const; 
	
	float GetEquipDuration() const; 
	
	// �quipement Animation et son
	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
	
	void StopWeaponAnimation(UAnimMontage* Animation); 
	
	//UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	void StartFire(); 
	void StopFire(); 
	
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerStartFire(); 
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerStopFire(); 
	
	void OnBurstStarted(); 
	void OnBurstFinished(); 
	
	virtual void HandleFiring(); // Pourra �tre surcharg�e dans les armes 
	bool CanFire() const; 
	
	virtual void SimulateWeaponFire();
	virtual void StopSimulatingWeaponFire();
	UFUNCTION(Reliable, Server, WithValidation)
	void SimulateWeaponFireServer();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void SpawnMuzzleEffectsMulticast();
	
	/* Avec PURE_VIRTUAL, nous n'avons pas � implanter la fonction ici, 
	nous l'implanterons dans les classes d�riv�es */ 
	virtual void FireWeapon() PURE_VIRTUAL(AWeapon::FireWeapon, ); 
	
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerHandleFiring(); 

	void OnReload();
	void OnReloadFinished();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerReload();
	
	UFUNCTION() 
	void OnRep_BurstCounter(); 

	UFUNCTION()
	void OnRep_bPendingReload();
	
	FVector GetMuzzleLocation() const; 
	FVector GetMuzzleDirection() const;

	EAmmoType GetAmmoType();
	EWeaponType GetWeaponType();

	/** Obtenir le mesh de l'arme */ 
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon") 
	USkeletalMeshComponent* GetWeaponMesh() const;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION() 
	void OnRep_MyPawn();

	int getMagCurrent();

	FVector GetAdjustedAim() const;
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerNotifyHit(const FHitResult Impact,/* FVector_NetQuantizeNormal Origin,*/ FVector_NetQuantizeNormal ShootDir);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerNotifyMiss(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir);

	void ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	bool ShouldDealDamage(AActor* TestActor) const;
	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	void SimulateInstantHit(const FVector& Origin);

	UFUNCTION(Reliable, Server, WithValidation)
	void SimulateInstantHitServer(const FVector& Origin);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void SpawnImpactEffectsMulticast(const FHitResult& Impact);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void SpawnTrailEffectsMulticast(const FVector& EndPoint);

protected:
	void UpdateSpreadVector();
};