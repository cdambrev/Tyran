// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/item/Loot.h"
#include "Basic/TyranController.h"
#include "TyranGameMode.generated.h"

UCLASS(minimalapi)
class ATyranGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<ALoot>> availableLoot;

	UPROPERTY(BlueprintReadOnly)
	ATyranController * tyranController;

	UPROPERTY()
	TSubclassOf<APawn> defaultRebelPawn;

	UPROPERTY()
	TSubclassOf<APawn> defaultTyranPawn;

	ATyranGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
};



