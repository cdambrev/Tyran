// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/item/Loot.h"
#include "Basic/TyranController.h"
#include "GUI/TyranHUD.h"
//#include "GUI/RevHUD.h"
#include "AI/HeatMap.h"
#include "TyranGameMode.generated.h"

UCLASS(minimalapi)
class ATyranGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UHeatMap * heatMap;

	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<ALoot>> availableLoot;

	UPROPERTY(BlueprintReadOnly)
	ATyranController * tyranController;

	UPROPERTY()
	TSubclassOf<APawn> defaultRebelPawn;

	UPROPERTY()
	TSubclassOf<APawn> defaultTyranPawn;

	UPROPERTY()
	TSubclassOf<ATyranHUD> TyranHUD;
	
	//UPROPERTY()
	//TSubclassOf<ARevHUD> RevHUD;

	ATyranGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BluePrintCallable) 
	UTexture2D * getHeatMapTex(){
		return heatMap->getTexture();
	}
};



