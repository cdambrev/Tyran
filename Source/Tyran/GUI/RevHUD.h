// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <UserWidget.h>
#include "../Basic/CaptureMiniMap.h"
#include "RevHUD.generated.h"

/**
* 
*/
UCLASS()
class TYRAN_API ARevHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	// widget par d�faut
	TSubclassOf<UUserWidget> defaultUIClass;
	UUserWidget* defaultUIWidget;
	bool updateMapNextTick = true;
	
	// widget pour l'inventaire plein
	TSubclassOf<UUserWidget> inventoryFullUIClass;
	UUserWidget* inventoryFullUIWidget;

public:
	UPROPERTY()
		TSubclassOf<ACaptureMiniMap> defaultCapture;
	UPROPERTY()
		ACaptureMiniMap * captureMap;

public:

	ARevHUD();

	void BeginPlay() override;

	void moveMiniMap();

	void updateMap();

	void setMapUpdateState(bool updateNextTick);

	void displayDefaultUI();

	void removeDefaultUI();

	void drawInventoryFull();

	void Tick(float DeltaSeconds) override;
	
};
