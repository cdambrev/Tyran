// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/TyranOnly/Placeable_Object/Building.h"
#include "Gameplay/TyranOnly/Placeable_Object/PlaceableObject.h"
#include "Gameplay/TyranOnly/Placeable_Object/BuildingHint.h"
#include "Gameplay/TyranOnly/Placeable_Object/BuildingSlot.h"
#include "GuardCharacter.h"
#include "AI/AIGuardTargetPoint.h"
#include "ManagerViewPawn.generated.h"

UCLASS()
class TYRAN_API AManagerViewPawn : public APawn
{
	GENERATED_BODY()

private :
	//Input variables
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFocaleFactor;
	//float ZoomFactor;
	bool bZoomingIn;
	bool bActivatePitchYawn;
	// Focus (par le clic)
	AActor* focus;
	TArray<FVector> patrolPoints;

public:
	enum ManagerState {
		NOTHING,
		BUILDING,
		PLACINGOBJECT,
		FOCUSGARDE,
		ORDERMENU,
		PATROLPOINTS,
		ZONETRACE,
		VECTORTRACE
	};


	// Sets default values for this pawn's properties
	AManagerViewPawn();

	/** Camera Component */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* RTSCamera;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* RTSCameraSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* RTSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Camera")
	float SpringArmLength = 30.f;

	ManagerState currState = NOTHING;

	ABuildingHint * currBuild = nullptr;
	TSubclassOf<ABuilding> buildClass;
	TSubclassOf<APlaceableObject> objectClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*input*/
	void MoveForward(float Value);

	void MoveRight(float Value);

	void PitchCamera(float Value);

	void YawCamera(float Value);

	void ZoomInFocale();

	void ZoomOutFocale();


	void ActivatePitchYawn();

	void DesactivatePitchYawn();
	

	void ZoomIn();

	void ZoomOut();

	void FastMoveInput(float Direction);
	
	UFUNCTION(BlueprintCallable)
	void enterBuildMode(TSubclassOf<ABuilding> building, TSubclassOf<ABuildingHint> buildHint);

	UFUNCTION(BlueprintCallable)
	void enterPlaceMode(TSubclassOf<APlaceableObject> object, TSubclassOf<ABuildingHint> buildHint);

	void leftClickAction();

	void RightClickAction();

	UFUNCTION(Reliable, Server, WithValidation)
	void callBuildOnSlot(ABuildingSlot * slot, TSubclassOf<ABuilding> buildClass);

	UFUNCTION(Reliable, Server, WithValidation)
	void placeObject(FTransform position, TSubclassOf<APlaceableObject> objectClass);

	UFUNCTION(Reliable, Server, WithValidation)
	void orderPatrolPoints(AActor* garde, const TArray<FVector>& patrolPoints);

	UFUNCTION(BlueprintCallable)
	void terminatePatrolPoints();

	UFUNCTION(BlueprintCallable)
	void offensifChecked();

	UFUNCTION(BlueprintCallable)
	void defensifChecked();

	UFUNCTION(BlueprintCallable)
	void tenirPositionChecked();

	UFUNCTION(BlueprintCallable)
	void fuiteAutoriseChecked(bool isChecked);
	
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	void offensifCheckedServ(AGuardCharacter* guard);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	void defensifCheckedServ(AGuardCharacter* guard);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	void tenirPositionCheckedServ(AGuardCharacter* guard);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	void fuiteAutoriseCheckedServ(AGuardCharacter* guard, bool isChecked);

	UFUNCTION(BlueprintCallable)
	void enterSetPatrouilleMode();

	UFUNCTION(BlueprintCallable)
	void enterSetZoneSurveillanceMode();

	UFUNCTION(BlueprintCallable)
	void enterPositionAndDirectionSelectionMode();

	bool clickOnGuard(FVector& mouseLocation, FVector& mouseDirection, FHitResult& resultHit);

	UFUNCTION(BlueprintCallable)
	AActor* getFocus();

	UFUNCTION(BlueprintCallable)
	void modificationOffensifCheckedInfo();
	
	UFUNCTION(BlueprintCallable)
	void modificationDefensifCheckedInfo();
	
	UFUNCTION(BlueprintCallable)
	void modificationTenirPosCheckedInfo();
	
	UFUNCTION(BlueprintCallable)
	void modificationFuiteCheckedInfo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
