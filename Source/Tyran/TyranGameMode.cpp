// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TyranGameMode.h"
#include "TyranCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ManagerViewPawn.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "ManagerPlayerState.h"

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
	if (NewPlayer->IsLocalController()) {
		//Action for server player (spectator ?)
	}
	else {
		ATyranController * player = static_cast<ATyranController *>(NewPlayer);
		TActorIterator<APlayerStart> spawnPoints(GetWorld());
		if (!tyranController) {
			tyranController = player;
			player->PlayerState = GetWorld()->SpawnActor<AManagerPlayerState>(AManagerPlayerState::StaticClass(), FTransform());
			player->setTyran(true);
			//AManagerViewPawn * tyranPawn = GetWorld()->SpawnActor<AManagerViewPawn>(defaultTyranPawn,FTransform((*spawnPoints)->GetActorLocation()));
			AManagerViewPawn * tyranPawn = GetWorld()->SpawnActor<AManagerViewPawn>(defaultTyranPawn, FTransform(FVector(-3370.0f, 1090.0f, 1220.0f)));
			player->Possess(tyranPawn);
		}
		else {
			player->setTyran(false);
			ATyranCharacter * revChar = GetWorld()->SpawnActor<ATyranCharacter>(defaultRebelPawn, FTransform((*spawnPoints)->GetActorLocation()));
			player->Possess(revChar);
		}
	}
}
