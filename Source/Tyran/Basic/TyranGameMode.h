// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
<<<<<<< HEAD:Source/Tyran/Basic/TyranGameMode.h
#include "TyranController.h"
#include "Loot.h"
=======
#include "Basic/TyranController.h"
>>>>>>> debug:Source/Tyran/Basic/TyranGameMode.h
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



