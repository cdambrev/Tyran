// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerViewPawn.h"
#include "GameFramework/Controller.h"
#include "TyranCharacter.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AIGuardTargetPoint.h"
#include "AIGuardController.h"
#include "EngineUtils.h"
#include "Components/InputComponent.h"


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
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
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
	
	// Clic gauche
	PlayerInputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &AManagerViewPawn::onLeftClick);

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


void AManagerViewPawn::onLeftClick() {
	FHitResult hit;
	APlayerController* tyranController = Cast<APlayerController>(GetController());
	if (tyranController != nullptr)
	{
		tyranController->GetHitResultUnderCursor(ECC_WorldDynamic, false, hit);

		if (hit.bBlockingHit) {
			// Clic sur un garde
			ATyranCharacter *guard = dynamic_cast<ATyranCharacter*>(hit.GetActor());
			if (guard && guard->getAlignement() == EAlignement::A_TYRAN) {
				focus = guard;
			}
			else {
				tyranController->GetHitResultUnderCursor(ECC_WorldStatic, false, hit);
				AAIGuardController* aiGuardController = Cast<AAIGuardController>(focus);
				if (hit.bBlockingHit && focus) {
					int targetPointNumber = 0;
					for (TActorIterator<AAIGuardTargetPoint> it(GetWorld()); it; ++it) {
						// Le targetPoint à traiter
						AAIGuardTargetPoint* targetPoint = *it;
						if (targetPointNumber == targetPoint->Position) {
							targetPoint->SetActorLocation(hit.ImpactPoint);
						}
					}
				}
			}
		}
	}
}

void AManagerViewPawn::FastMoveInput(float Direction) {

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
		PC->GetMousePosition(LocationX, LocationY);
		PC->GetViewportSize(SizeX, SizeY);

		//edge
		if (!bActivatePitchYawn) {

			float posX = LocationX / SizeX;
			float posY = LocationY / SizeY;

			if (posX > 0.95f) {
				MoveRight(1.0f);
			}
			else if (posX < 0.05f) {
				MoveRight(-1.0f);
			}
			if (posY > 0.95f) {
				MoveForward(-1.0f);
			}
			else if (posY < 0.05f) {
				MoveForward(1.0f);
			}


			
		}
		
		//PitchYawn
		else{
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
}
