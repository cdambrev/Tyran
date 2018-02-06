// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TyranController.h"
#include "TyranGameMode.generated.h"

UCLASS(minimalapi)
class ATyranGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	ATyranController * tyranController;

	UPROPERTY()
	TSubclassOf<APawn> defaultRebelPawn;

	UPROPERTY()
	TSubclassOf<APawn> defaultTyranPawn;

public:
	ATyranGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

};



