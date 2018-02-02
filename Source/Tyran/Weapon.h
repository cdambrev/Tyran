// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TyranCharacter.h"
#include "Sound/SoundCue.h"
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

protected:
	/** Le propri�taire */ 
	UPROPERTY(Transient) 
	ATyranCharacter* MyPawn;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	/* Le socket (emplacement) o� l'objet sera plac� */ 
	EInventorySlot StorageSlot;

	// ******* �quipement 
	/** D�part de l'�quipement */ 
	float EquipStartedTime; 
	
	/** Dur�e de l'�quipement */ 
	float EquipDuration; 
	
	bool bIsEquipped;
	
	bool bPendingEquip; 
	
	FTimerHandle EquipFinishedTimerHandle;
	
	EWeaponState CurrentState;
		
	// Animations et son 
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds") 
	USoundCue* EquipSound;

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

	/** Obtenir le mesh de l'arme */ 
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon") 
	USkeletalMeshComponent* GetWeaponMesh() const;
};