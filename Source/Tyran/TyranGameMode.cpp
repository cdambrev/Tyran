// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TyranGameMode.h"
#include "TyranCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ManagerViewPawn.h"

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
	//AGameModeBase::PostLogin(NewPlayer);
	ATyranController * player = static_cast<ATyranController *>(NewPlayer);
	if (!tyranController) {
		tyranController = player;
		player->setTyran(true);
		AManagerViewPawn * tyranPawn = GetWorld()->SpawnActor<AManagerViewPawn>(defaultTyranPawn,FTransform(FVector(200.0f,873.0f,232.0f)));
		player->Possess(tyranPawn);
	}
	else {
		ATyranCharacter * revChar = GetWorld()->SpawnActor<ATyranCharacter>(defaultRebelPawn, FTransform(FVector(200.0f, 873.0f, 232.0f)));
		player->Possess(revChar);
	}
}
