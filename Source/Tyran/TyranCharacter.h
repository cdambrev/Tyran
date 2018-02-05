// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "GameFramework/Character.h"
#include "TyranTypes.h"
#include "Net/UnrealNetwork.h"
#include "TyranCharacter.generated.h"

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

public:
	ATyranCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mouvement") 
	bool CrouchButtonDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mouvement")
	bool JumpButtonDown;

	/* Inventaire des armes */
	UPROPERTY(Transient, Replicated)
	TArray<class AWeapon*> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Replicated)
	bool isVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAlwaysVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int timeBeforeDisapear;

protected:
	/* Point d'attache pour les items en main et actifs */ 
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint; 
	
	/* Point d'attache pour les items à la ceinture. */ 
	UPROPERTY(EditDefaultsOnly, Category = "Sockets") 
	FName PelvisAttachPoint; 
	
	/* Point d'attache pour l'arme principale */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName SpineAttachPoint;

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

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Retourne le point d'attache (socket) pour correspondre au socket du squelette */ 
	FName GetInventoryAttachPoint(EInventorySlot Slot) const;

	void AddWeapon(class AWeapon* Weapon);

	void EquipWeapon(class AWeapon* Weapon);

	virtual void PostInitializeComponents() override;

	void SpawnDefaultInventory();

	void SetCurrentWeapon(class AWeapon* NewWeapon, class AWeapon* LastWeapon = nullptr); 

	void OnStartFire(); 
	
	void OnStopFire();

	void StartWeaponFire(); 
	
	void StopWeaponFire();

	bool CanFire() const;
	
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipWeapon(AWeapon* Weapon);

	/* La fonction OnRep utilise un paramètre pour la valeur précédente de la variable */ 
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Tyran")
	void setVisible(bool b);

	UFUNCTION(BlueprintCallable, Category="Tyran")
	void setViewedThisTick();

	void Tick(float DeltaSeconds) override;
};

