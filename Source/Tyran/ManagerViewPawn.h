// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Building.h"
#include "BuildingHint.h"
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
public:
	// Sets default values for this pawn's properties
	AManagerViewPawn();

	/** Camera Component */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* RTSCamera;*/

	UPROPERTY(EditAnywhere)
	USpringArmComponent* RTSCameraSpringArm;
	UCameraComponent* RTSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Camera")
	float SpringArmLength = 400.f;

	bool buildMode = false;

	ABuildingHint * currBuild = nullptr;
	TSubclassOf<ABuilding> buildClass;

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

	void ZoomIn();

	void ZoomOut();

	void FastMoveInput(float Direction);

	UFUNCTION(BlueprintCallable)
	void enterBuildMode(TSubclassOf<ABuilding> building, TSubclassOf<ABuildingHint> buildHint);

	void leftClickAction();

	void RightClickAction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
