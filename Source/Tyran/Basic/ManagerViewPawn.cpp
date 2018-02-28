// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerViewPawn.h"
#include "GameFramework/Controller.h"
#include "TyranCharacter.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"
#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "GuardCharacter.h"
#include "Basic/ManagerPlayerState.h"
#include "AI/AIGuardTargetPoint.h"
#include <ConstructorHelpers.h>
#include <UserWidget.h>
#include "GUI/TyranHUD.h"
#include "Gameplay/item/Trap/Trap.h"
#include "TyranController.h"


/***************/
/**Constructor**/
/***************/


// Sets default values
AManagerViewPawn::AManagerViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	RTSCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	RTSCameraSpringArm->SetupAttachment(RootComponent);
	RTSCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	RTSCameraSpringArm->TargetArmLength = SpringArmLength;
	RTSCameraSpringArm->bEnableCameraLag = true;
	RTSCameraSpringArm->CameraLagSpeed = 3.0f;

	RTSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RTSCamera"));
	RTSCamera->SetupAttachment(RTSCameraSpringArm, USpringArmComponent::SocketName);

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AManagerViewPawn::BeginPlay()
{
	Super::BeginPlay();
	//APlayerController* PC = Cast<APlayerController>(GetController());

	//if (PC)
	//{
	//	PC->bShowMouseCursor = true;
	//	PC->bEnableClickEvents = true;
	//	PC->bEnableMouseOverEvents = true;
	//}
}

/********/
/**Bind**/
/********/

// Called to bind functionality to input
void AManagerViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AManagerViewPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AManagerViewPawn::MoveRight);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AManagerViewPawn::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AManagerViewPawn::ZoomOut);

	//PlayerInputComponent->BindAction("ZoomFocale", IE_Pressed, this, &AManagerViewPawn::ZoomInFocale);
	//PlayerInputComponent->BindAction("ZoomFocale", IE_Released, this, &AManagerViewPawn::ZoomOutFocale);

	PlayerInputComponent->BindAction("ActivatePitchYawn", IE_Pressed, this, &AManagerViewPawn::ActivatePitchYawn);
	PlayerInputComponent->BindAction("ActivatePitchYawn", IE_Released, this, &AManagerViewPawn::DesactivatePitchYawn);

	//Hook up every-frame handling for our four axes
	PlayerInputComponent->BindAxis("MoveForward", this, &AManagerViewPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AManagerViewPawn::MoveRight);
	PlayerInputComponent->BindAxis("CameraPitch", this, &AManagerViewPawn::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &AManagerViewPawn::YawCamera);

	// double speed (WASD +Shift)
	PlayerInputComponent->BindAxis("FastMove", this, &AManagerViewPawn::FastMoveInput);
	
	PlayerInputComponent->BindAction("TyranSelectAndAct", IE_Pressed, this, &AManagerViewPawn::leftClickAction);
	PlayerInputComponent->BindAction("TyranOrderAndCancel", IE_Pressed, this, &AManagerViewPawn::RightClickAction);

}


/************/
/**Movement**/
/************/

/** Called for forwards/backward input */
void AManagerViewPawn::MoveForward(float Value){
	MovementInput.X = FMath::Clamp<float>(Value, -1.0f, 1.0f);
}

/** Called for side to side input */
void AManagerViewPawn::MoveRight(float Value){
	MovementInput.Y = FMath::Clamp<float>(Value, -1.0f, 1.0f);
}

void AManagerViewPawn::PitchCamera(float Value)
{
	CameraInput.Y = Value;
}

void AManagerViewPawn::YawCamera(float Value)
{
	CameraInput.X = Value;
}

void AManagerViewPawn::ZoomInFocale()
{
	bZoomingIn = true;
}

void AManagerViewPawn::ZoomOutFocale()
{
	bZoomingIn = false;
}

void AManagerViewPawn::ActivatePitchYawn()
{
	bActivatePitchYawn = true;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
	}
}

void AManagerViewPawn::DesactivatePitchYawn()
{
	bActivatePitchYawn = false;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

void AManagerViewPawn::ZoomIn()
{
	//ZoomFocaleFactor += 0.1f;
	FVector frontCamera = RTSCamera->GetComponentRotation().Vector();
	FVector NewLocation = GetActorLocation();
	NewLocation += frontCamera * 150;
	SetActorLocation(NewLocation);
}

void AManagerViewPawn::ZoomOut()
{
	//ZoomFocaleFactor -= 0.1f;
	FVector frontCamera = RTSCamera->GetComponentRotation().Vector();
	FVector NewLocation = GetActorLocation();
	NewLocation -= frontCamera * 250 ;
	SetActorLocation(NewLocation);
}

void AManagerViewPawn::FastMoveInput(float Direction) {

}

void AManagerViewPawn::enterBuildMode(TSubclassOf<ABuilding> building, TSubclassOf<ABuildingHint> buildHint)
{
	if (currState == BUILDING || currState == PLACINGOBJECT) {
		RightClickAction();
	}
	currBuild = GetWorld()->SpawnActor<ABuildingHint>(buildHint, GetTransform());
	buildClass = building;
	currState = BUILDING;
}

void AManagerViewPawn::enterPlaceMode(TSubclassOf<APlaceableObject> object, TSubclassOf<ABuildingHint> buildHint)
{
	if (currState == BUILDING || currState == PLACINGOBJECT) {
		RightClickAction();
	}
	currBuild = GetWorld()->SpawnActor<ABuildingHint>(buildHint, GetTransform());
	objectClass = object;
	currState = PLACINGOBJECT;
}

void AManagerViewPawn::offensifChecked() {
	offensifCheckedServ(static_cast<AGuardCharacter*>(focus));
}

void AManagerViewPawn::defensifChecked() {
	defensifCheckedServ(static_cast<AGuardCharacter*>(focus));
}
void AManagerViewPawn::tenirPositionChecked() {
	tenirPositionCheckedServ(static_cast<AGuardCharacter*>(focus));
}
void AManagerViewPawn::fuiteAutoriseChecked(bool isChecked) {
	fuiteAutoriseCheckedServ(static_cast<AGuardCharacter*>(focus), isChecked);
}

bool AManagerViewPawn::offensifCheckedServ_Validate(AGuardCharacter* guard) {
	return true;
}
void AManagerViewPawn::offensifCheckedServ_Implementation(AGuardCharacter* guard) {
	(static_cast<AGuardCharacter*> (guard))->modeGuard = ModeGuard::OFFENSIF;
}

bool AManagerViewPawn::defensifCheckedServ_Validate(AGuardCharacter* guard) {
	return true;
}
void AManagerViewPawn::defensifCheckedServ_Implementation(AGuardCharacter* guard) {
	(static_cast<AGuardCharacter*> (guard))->modeGuard = ModeGuard::DEFENSIF;
}

bool AManagerViewPawn::tenirPositionCheckedServ_Validate(AGuardCharacter* guard) {
	return true;
}
void AManagerViewPawn::tenirPositionCheckedServ_Implementation(AGuardCharacter* guard) {
	(static_cast<AGuardCharacter*> (guard))->modeGuard = ModeGuard::TENIRPOSITION;
}

bool AManagerViewPawn::fuiteAutoriseCheckedServ_Validate(AGuardCharacter* guard, bool isChecked) {
	return true;
}
void AManagerViewPawn::fuiteAutoriseCheckedServ_Implementation(AGuardCharacter* guard, bool isChecked) {
	(static_cast<AGuardCharacter*> (guard))->fuiteAutorise = isChecked;
}

void AManagerViewPawn::enterSetPatrouilleMode() {
	currState = PATROLPOINTS;
	static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayPatrolPointsMode();
	static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardOrder();
}

void AManagerViewPawn::enterSetZoneSurveillanceMode() {

}

void AManagerViewPawn::enterPositionAndDirectionSelectionMode() {

}

void AManagerViewPawn::leftClickAction()
{
	if (currState == FOCUSGARDE) {
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (clickOnGuard(mouseLocation, mouseDirection, resultHit)) {
			currState = ORDERMENU;
			AGuardCharacter* guard = dynamic_cast<AGuardCharacter*>(resultHit.GetActor());
			focus = guard;
			float mouseScreenX;
			float mouseScreenY;
			static_cast<APlayerController*>(GetController())->GetMousePosition(mouseScreenX, mouseScreenY);
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardOrder();
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardOrder(FVector2D(mouseScreenX, mouseScreenY));
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardInfo();
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardInfo();
		} else {
			currState = NOTHING;
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardInfo();
			focus->Destroy();
		}
	}
	else if (currState == PATROLPOINTS) {
		FHitResult hitResult{};
		mouseRaycast(hitResult, ECollisionChannel::ECC_GameTraceChannel4);
		patrolPoints.Add(hitResult.ImpactPoint);
		
	} else if (currState == ORDERMENU) {
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (clickOnGuard(mouseLocation, mouseDirection, resultHit)) {
			AGuardCharacter* guard = dynamic_cast<AGuardCharacter*>(resultHit.GetActor());
			focus = guard;
			float mouseScreenX;
			float mouseScreenY;
			static_cast<APlayerController*>(GetController())->GetMousePosition(mouseScreenX, mouseScreenY);
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardOrder();
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardOrder(FVector2D(mouseScreenX, mouseScreenY));
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardInfo();
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardInfo();
		} else {
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardOrder();
			currState = FOCUSGARDE;
		}

	} else if (currState == BUILDING) {
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (static_cast<APlayerController *>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
			if (GetWorld()->LineTraceSingleByChannel(resultHit, mouseLocation, mouseLocation + 100000 * mouseDirection, ECollisionChannel::ECC_GameTraceChannel1, queryParams)) {
				if (resultHit.Actor.IsValid() && (&*resultHit.Actor)->IsA(ABuildingSlot::StaticClass())) {
					callBuildOnSlot(static_cast<ABuildingSlot *>(&*resultHit.Actor), buildClass);
					currState = NOTHING;
					currBuild->Destroy();
				}
				else {
					currBuild->TeleportTo(resultHit.ImpactPoint, FRotator{ 0,0,0 });
					currBuild->setValidPosition(false);
				}
			}
		}
	} else if (currState == PLACINGOBJECT) {
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		queryParams.AddIgnoredActor(currBuild);
		queryParams.bTraceComplex = true;
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (static_cast<APlayerController *>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
			if (GetWorld()->LineTraceSingleByChannel(resultHit, mouseLocation, mouseLocation + 100000 * mouseDirection, ECollisionChannel::ECC_Visibility, queryParams)) {
				currBuild->TeleportTo(resultHit.ImpactPoint, resultHit.Normal.Rotation() - FRotator(90.0f, 0.0f, 0.0f));
				if (currBuild->checkValidPosition()) {
					placeObject(FTransform(resultHit.Normal.Rotation() - FRotator(90.0f, 0.0f, 0.0f), resultHit.ImpactPoint), objectClass);
					currState = NOTHING;
					currBuild->Destroy();
				}
			}
		}
	}
	else {
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (clickOnGuard(mouseLocation, mouseDirection, resultHit)) {
			AGuardCharacter* guard = dynamic_cast<AGuardCharacter*>(resultHit.GetActor());
			currState = ORDERMENU;
			focus = guard;
			float mouseScreenX;
			float mouseScreenY;
			static_cast<APlayerController*>(GetController())->GetMousePosition(mouseScreenX, mouseScreenY);
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardOrder(FVector2D(mouseScreenX, mouseScreenY));
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->displayGuardInfo();
		}
	}
}

void AManagerViewPawn::mouseRaycast(FHitResult & hitResult, ECollisionChannel colChannel) {
	FVector mouseLocation;
	FVector mouseDirection;
	FCollisionQueryParams queryParams{};
	queryParams.AddIgnoredActor(GetOwner());
	if (static_cast<APlayerController *>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
		GetWorld()->LineTraceSingleByChannel(hitResult, mouseLocation, mouseLocation + 100000 * mouseDirection, colChannel, queryParams);
	}
}




void AManagerViewPawn::RightClickAction()
{
	if (currState == BUILDING || currState == PLACINGOBJECT) {
		currState = NOTHING;
		currBuild->Destroy();
	}
	else if (currState == ORDERMENU) {
		static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardOrder();
		static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardInfo();
		currState = NOTHING;
		focus->Destroy();
	}
	else if (currState == FOCUSGARDE) {
		currState = NOTHING;
		static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removeGuardInfo();
		focus->Destroy();
	}
	else if (currState == PATROLPOINTS) {
		if (patrolPoints.Num() != 0) {
			patrolPoints.RemoveAt(patrolPoints.Num() - 1);
		}
		else {
			static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removePatrolPointsMode();
			currState = FOCUSGARDE;
		}
	}
}

void AManagerViewPawn::callBuildOnSlot_Implementation(ABuildingSlot * slot, TSubclassOf<ABuilding> tBuildClass)
{
	if (static_cast<AManagerPlayerState *>(GetController()->PlayerState)->spendMoney(static_cast<ABuilding *>(tBuildClass->ClassDefaultObject)->basePrice)) {
		slot->build(tBuildClass);

		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ATyranController * c = Cast<ATyranController>(Iterator->Get());
			if (c)
			{
				if (!c->isTyran)
					c->setMapUpdateState(true);
			}
		}
	}
}

bool AManagerViewPawn::callBuildOnSlot_Validate(ABuildingSlot * slot, TSubclassOf<ABuilding> tBuildClass)
{
	return true;
}

void AManagerViewPawn::placeObject_Implementation(FTransform position, TSubclassOf<APlaceableObject> tObjectClass) {
	if (static_cast<AManagerPlayerState *>(GetController()->PlayerState)->spendMoney(static_cast<APlaceableObject *>(tObjectClass->ClassDefaultObject)->basePrice)) {
		auto obj = GetWorld()->SpawnActor<APlaceableObject>(tObjectClass, position);
		if (ATrap* trap = Cast<ATrap>(obj))
		{
			trap->SetOwningPawn(this);
		}
		
	}
}

bool AManagerViewPawn::placeObject_Validate(FTransform position, TSubclassOf<APlaceableObject> tObjectClass) {
	return true;
}

void AManagerViewPawn::orderPatrolPoints_Implementation(AActor* garde, const TArray<FVector>& patrolPointsPos) {
	AAIGuardController* aiGuardController = Cast<AAIGuardController>(garde->GetInstigatorController());
	if (garde) {
		TArray<AAIGuardTargetPoint*> guardTargetPoints;
		for (int i = 0; i < patrolPointsPos.Num(); ++i) {
			FVector targetPointPos = patrolPointsPos[i];
			AAIGuardTargetPoint* targetPoint = GetWorld()->SpawnActor<AAIGuardTargetPoint>(AAIGuardTargetPoint::StaticClass(), FTransform(targetPointPos));
			targetPoint->SetActorLocation(targetPointPos);
			targetPoint->Position = i;
			guardTargetPoints.Add(targetPoint);
		}
		aiGuardController->setPatrolPoint(guardTargetPoints);
	}
}

bool AManagerViewPawn::orderPatrolPoints_Validate(AActor* garde, const TArray<FVector>& patrolPointsPos) {
	return true;
}

void AManagerViewPawn::terminatePatrolPoints() {
	orderPatrolPoints(focus, patrolPoints);
	static_cast<ATyranHUD*>(static_cast<APlayerController*>(GetController())->GetHUD())->removePatrolPointsMode();
	patrolPoints.Empty();
	currState = FOCUSGARDE;
}

// Called every frame
void AManagerViewPawn::Tick(float DeltaTime)
{
	//Zoom Zoom
	{
		if (bZoomingIn)
		{
			ZoomFocaleFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFocaleFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		}
		ZoomFocaleFactor = FMath::Clamp<float>(ZoomFocaleFactor, 0.0f, 1.0f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFocaleFactor
		RTSCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFocaleFactor);
		RTSCameraSpringArm->TargetArmLength = FMath::Lerp<float>(SpringArmLength, 0.75*SpringArmLength, ZoomFocaleFactor);
	}


	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) {
		float LocationX;
		float LocationY;
		int SizeX;
		int SizeY;
		bool onViewPort = PC->GetMousePosition(LocationX, LocationY);
		PC->GetViewportSize(SizeX, SizeY);

		//edge
		if (!bActivatePitchYawn && onViewPort) {

			float posX = LocationX / SizeX;
			float posY = LocationY / SizeY;
			float edgeLength = 0.005f; // 0.01f = 1% of viewport

			if (posX > 1.0f- edgeLength) {
				MoveRight(1.0f);
			}
			else if (posX < edgeLength) {
				MoveRight(-1.0f);
			}
			if (posY > 1.0f - edgeLength) {
				MoveForward(-1.0f);
			}
			else if (posY < edgeLength) {
				MoveForward(1.0f);
			}
		}
		
		//PitchYawn
		else if (bActivatePitchYawn){
			PC->SetMouseLocation(SizeX / 2.0, SizeY / 2.0);
			//Rotate our actor's yaw, which will turn our camera because we're attached to it
			{
				FRotator NewRotation = GetActorRotation();
				NewRotation.Yaw += CameraInput.X;
				SetActorRotation(NewRotation);
			}
			//Rotate our camera's pitch, but limit it so we're always looking downward
			{
				FRotator NewRotation = RTSCamera->GetComponentRotation();
				NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
				RTSCameraSpringArm->SetWorldRotation(NewRotation);
			}
		}
	}
	//Check Your Booty (Movement)
	{
		//move
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 3000 units per second
			MovementInput = MovementInput.GetSafeNormal() * 3000.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}

	if (currState == BUILDING) {
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (static_cast<APlayerController *>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
			if (GetWorld()->LineTraceSingleByChannel(resultHit, mouseLocation, mouseLocation + 100000*mouseDirection, ECollisionChannel::ECC_GameTraceChannel1, queryParams)) {
				if (resultHit.Actor.IsValid() && (&*resultHit.Actor)->IsA(ABuildingSlot::StaticClass())) {
					currBuild->TeleportTo((&*resultHit.Actor)->GetActorLocation(), (&*resultHit.Actor)->GetActorRotation());
					currBuild->setValidPosition(true);
				}
				else {
					currBuild->TeleportTo(resultHit.ImpactPoint, FRotator{ 0,0,0 });
					currBuild->setValidPosition(false);
				}
			}
		}
	}

	if (currState == PLACINGOBJECT) {
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		queryParams.AddIgnoredActor(currBuild);
		queryParams.bTraceComplex = true;
		FHitResult resultHit{};
		FVector mouseLocation;
		FVector mouseDirection;
		if (static_cast<APlayerController *>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection)) {
			if (GetWorld()->LineTraceSingleByChannel(resultHit, mouseLocation, mouseLocation + 100000 * mouseDirection, ECollisionChannel::ECC_Visibility, queryParams)) {
				currBuild->TeleportTo(resultHit.ImpactPoint, resultHit.Normal.Rotation()-FRotator(90.0f,0.0f,0.0f));
				currBuild->checkValidPosition();
			}
		}
	}
}

bool AManagerViewPawn::clickOnGuard(FVector& mouseLocation, FVector& mouseDirection, FHitResult& resultHit) {
	FCollisionQueryParams queryParams{};
	queryParams.AddIgnoredActor(GetOwner());
	return (static_cast<APlayerController*>(GetController())->DeprojectMousePositionToWorld(mouseLocation, mouseDirection) &&
		GetWorld()->LineTraceSingleByChannel(resultHit, mouseLocation, mouseLocation + 100000 * mouseDirection, ECollisionChannel::ECC_GameTraceChannel3, queryParams) &&
		resultHit.Actor.IsValid() && resultHit.GetActor()->IsA(AGuardCharacter::StaticClass()));
}

AActor* AManagerViewPawn::getFocus() {
	return focus;
}
