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
	Equipping, // L'arme est en cours d'équipement (animation) 
	Reloading // L'arme est en recharge (animation) 
};

UCLASS(ABSTRACT, Blueprintable)
class TYRAN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	/* Classe a faire apparaître lorsque l'objet est lache */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Weapon")
	TSubclassOf<class AWeaponLoot> WeaponLootClass;

protected:
	/** Le propriétaire */ 
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ATyranCharacter* MyPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	/* Le socket (emplacement) où l'objet sera placé */ 
	EInventorySlot StorageSlot;

	EAmmoType AmmoType;

	// ******* Équipement 
	/** Départ de l'équipement */ 
	float EquipStartedTime; 
	
	/** Durée de l'équipement */ 
	float EquipDuration; 
	
	bool bIsEquipped;
	
	bool bPendingEquip; 
	
	FTimerHandle EquipFinishedTimerHandle;
		
	// ******* Rechargement 
	/** Départ du rechargement */
	float ReloadStartedTime;

	/** Durée du rechargement */
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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE EInventorySlot GetStorageSlot() { return StorageSlot; }

	/* Prendre en note le propriétaire */ 
	void SetOwningPawn(ATyranCharacter* NewOwner);

	/* Associer l'arme à une position d'inventaire*/
	void AttachMeshToPawn(EInventorySlot Slot = EInventorySlot::Hands);	

	/* Nous détachons le mesh de l'acteur - operation 3d */
	void DetachMeshFromPawn();

	virtual void OnEnterInventory(ATyranCharacter* NewOwner);

	virtual void OnLeaveInventory();

	bool IsEquippedOrPending() const;

	virtual void OnUnEquip();

	// Équipement 
	void OnEquip(); 
	
	void DetermineWeaponState(); 
	
	void SetWeaponState(EWeaponState NewState); 
	
	EWeaponState GetCurrentState() const; 
	
	bool IsEquipped() const; 
	
	virtual void OnEquipFinished(); 
	
	float GetEquipStartedTime() const; 
	
	float GetEquipDuration() const; 
	
	// Équipement Animation et son
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
	
	virtual void HandleFiring(); // Pourra être surchargée dans les armes 
	bool CanFire() const; 
	
	virtual void SimulateWeaponFire();
	virtual void StopSimulatingWeaponFire();
	UFUNCTION(Reliable, Server, WithValidation)
	void SimulateWeaponFireServer();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void SpawnMuzzleEffectsMulticast();
	
	/* Avec PURE_VIRTUAL, nous n'avons pas à implanter la fonction ici, 
	nous l'implanterons dans les classes dérivées */ 
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

	/** Obtenir le mesh de l'arme */ 
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon") 
	USkeletalMeshComponent* GetWeaponMesh() const;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION() 
	void OnRep_MyPawn();

	/* LOOT */
	/*void OnBeginFocus() override;
	void OnEndFocus() override;
	void OnUsed(APawn* InstigatorPawn) override;*/
};