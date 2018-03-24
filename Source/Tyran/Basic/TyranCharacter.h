// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "GameFramework/Character.h"
#include "Basic/Enum/TyranTypes.h"
#include "Net/UnrealNetwork.h"
#include "Basic/Enum/Alignement.h"
#include "Enum/StateRev.h"
#include "TyranCharacter.generated.h"

/*
UENUM(BlueprintType)
enum class EAlignement : uint8 {
	A_TYRAN UMETA(DisplayName="Tyran"),
	A_REVOLUTIONNAIRE UMETA(DisplayName="Revolutionnaire")
};*/


//class forwarding
class UInteractionComponent;

UCLASS(config=Game)
class ATyranCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Aim camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* AimCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* AimCamera;

	/** FPS camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPSCamera;

public:
	ATyranCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mouvement", ReplicatedUsing = OnRep_CrouchButtonDown)
	bool CrouchButtonDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mouvement")
	bool JumpButtonDown;

	/* Inventaire des armes */
	UPROPERTY(Transient, Replicated)
	TArray<class AWeapon*> Inventory;

	/* Munitions */
	UPROPERTY(Replicated)
	TArray<int> Ammunition;

	
	/*Trap status*/
	bool isTraced;
	bool isStun;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Replicated)
	bool isVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlwaysVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int timeBeforeDisapear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAlignement alignement;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool isDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool isAiming;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Etat")
	EStateRev currState = EStateRev::ADECOUVERT;
	
	/* Point d'attache pour les items en main et actifs */ 
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint_Rifle; 

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint_Handgun;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint_Shotgun;
	
	/* Point d'attache pour les items à la ceinture. */ 
	UPROPERTY(EditDefaultsOnly, Category = "Sockets") 
	FName PelvisAttachPoint; 
	
	/* Point d'attache pour l'arme principale */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SpineAttachPoint_Shotgun;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SpineAttachPoint;

	/* Point d'attache pour les items en main et actifs */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName HeadAttachPoint;

	/* Distance pour lacher un objet d'inventaire. */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory") 
	float DropItemDistance; 
	
	/* Armes de defaut */ 
	UPROPERTY(EditDefaultsOnly, Category = Inventory) 
	TArray<TSubclassOf<class AWeapon>> DefaultInventoryClasses;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon) 
	class AWeapon* CurrentWeapon;

	int timeSinceLastView;

	bool bWantsToFire;

	bool bHasNewFocus; // Seulement vrai lors de la première image avec un nouveau focus.
	UInteractionComponent* FocusedInteraction;

	// Distance maximale de focus sur les objets.
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction") 
	float MaxUseDistance;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitAnim;

	UPROPERTY(Transient, Replicated)
	TArray<class UUsableObject*> objectInventory;

protected:
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Quand la barre d'espacement est appuyée ou relâchée 
	void OnStartJump(); 
	void OnStopJump();

	// Quand la touche Crouch Toggle est appuyée 
	void OnCrouchToggle();

	// Quand la touche Aim est appuyée
	void OnStartAim();
	void OnStopAim();

	// Quand la touche Use est appuyée 
	void Use();

	// "Et là IL MEUUUUUUUURT !"
	void OnDeath();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// AActor
	

	UInteractionComponent* GetInteractionInView();



public:

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void regenerate(float hp);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAddObjectInInventory(TSubclassOf<UUsableObject> objectClass, int spot);
	
	bool addObjectInInventory(TSubclassOf<UUsableObject> objectClass);

	int findSpotInInventory();
	
	//void msgInventoryFull();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Retourne le point d'attache (socket) pour correspondre au socket du squelette */ 
	FName GetInventoryAttachPoint(EInventorySlot Slot, EWeaponType WeaponType) const;

	void AddWeapon(class AWeapon* Weapon);

	void EquipWeapon(class AWeapon* Weapon);

	void AddAmmo(EAmmoType AmmoType, int nbAmmo);

	UFUNCTION(Server, Reliable, WithValidation)
	void SeverAddAmmo(EAmmoType AmmoType, int nbAmmo);

	virtual void PostInitializeComponents() override;

	void SpawnDefaultInventory();

	void SetCurrentWeapon(class AWeapon* NewWeapon, class AWeapon* LastWeapon = nullptr); 

	void OnStartFire(); 
	
	void OnStopFire();

	// Quand la touche Reload est appuyée 
	void OnReload();

	void StartWeaponFire(); 
	
	void StopWeaponFire();

	bool CanFire() const;

	void OnNextWeapon(); 
	void OnPrevWeapon(); 

	void OnEquipPrimaryWeapon(); 
	void OnEquipSecondaryWeapon();

	void DropWeapon();
	void RemoveWeapon(class AWeapon* Weapon);

	void setDead() {
		currState = EStateRev::MORT;
	}

	void setAgonisant() {
		currState = EStateRev::AGONISANT;
	}

	void setADecouvert() {
		currState = EStateRev::ADECOUVERT;
	}

	void setACouvert() {
		currState = EStateRev::ACOUVERT;
	}

	void setTirACouvert() {
		currState = EStateRev::TIRANTACOUVERT;
	}

	EStateRev getState() {
		return currState;
	}

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerDropWeapon();

	// Invocation d'une RPC serveur pour actualiser l'état de crouching
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCrouchToggle(bool NewCrouching);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnStartAim();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerOnStopAim();
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipWeapon(AWeapon* Weapon);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastPlayAnim(UAnimMontage* Anim);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
	void MulticastStopAnim(UAnimMontage* Anim);

	/* La fonction OnRep utilise un paramètre pour la valeur précédente de la variable */ 
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);

	UFUNCTION()
	void OnRep_CrouchButtonDown();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Tyran")
	void setVisible(bool b);

	UFUNCTION(BlueprintCallable, Category="Tyran")
	void setViewedThisTick();

	UFUNCTION(BlueprintCallable, Category = "Tyran")
	EAlignement getAlignement();

	/* Vérifier si l'emplacement est libre */
	bool WeaponSlotAvailable(EInventorySlot CheckSlot);
	void Tick(float DeltaSeconds) override;

	void setTemporarilyVisible(float second);

	void setTemporarilyStun(float second);

	int getMagCurrent();

	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrentWeaponType();

	UFUNCTION()
	void OnUseObject1();
	UFUNCTION()
	void OnUseObject2();
	UFUNCTION()
	void OnUseObject3();
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUseObject1();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUseObject2();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUseObject3();

protected:
	void setTemporarilyVisibleDelayedImplementation();

	void setTemporarilyStunDelayedImplementation();
};

