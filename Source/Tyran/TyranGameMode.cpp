// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TyranGameMode.h"
#include "TyranCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ManagerViewPawn.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "Blueprint/UserWidget.h"

ATyranGameMode::ATyranGameMode()
{

	DefaultPawnClass = NULL;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		defaultRebelPawn = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_TyranController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> ManagerViewPawnBPClass(TEXT("/Game/Blueprints/BP_ManagerViewPawn"));
	if (ManagerViewPawnBPClass.Class != NULL)
	{
		defaultTyranPawn = ManagerViewPawnBPClass.Class;
	}

	tyranController = nullptr;
}

void ATyranGameMode::PostLogin(APlayerController * NewPlayer)
{
	
	ATyranController * player = static_cast<ATyranController *>(NewPlayer);
	TActorIterator<APlayerStart> spawnPoints(GetWorld());
	if (!tyranController) {
		tyranController = player;
		player->setTyran(true);
		//AManagerViewPawn * tyranPawn = GetWorld()->SpawnActor<AManagerViewPawn>(defaultTyranPawn,FTransform((*spawnPoints)->GetActorLocation()));
		AManagerViewPawn * tyranPawn = GetWorld()->SpawnActor<AManagerViewPawn>(defaultTyranPawn,FTransform(FVector(-3370.0f, 1090.0f, 1220.0f)));
		player->Possess(tyranPawn);
		
	}
	else {
		player->setTyran(false);
		ATyranCharacter * revChar = GetWorld()->SpawnActor<ATyranCharacter>(defaultRebelPawn, FTransform((*spawnPoints)->GetActorLocation()));
		player->Possess(revChar);
	}

}

void ATyranGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}
	//TimeLeft = GameDuration;
}

FTimespan  ATyranGameMode::GetTimeLeft() const {
	return TimeLeft;
}

void ATyranGameMode::UpdateTimeLeft(const FTimespan& deltat) {
	if (TimeLeft>0)
	{
		TimeLeft -= deltat;
	}
}



void ATyranGameMode::InitTimeLeft(const FTimespan& init) {
	TimeLeft = init;
}
