// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <UserWidget.h>
#include "TyranHUD.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ATyranHUD : public AHUD
{
	GENERATED_BODY()
private:
	// widget pour les ordres des gardes
	UUserWidget* guardOrderWidget;
	TSubclassOf<UUserWidget> guardUIClass;

	// widget pour les patrolPoints
	UUserWidget* patrolPointsMode;
	TSubclassOf<UUserWidget> patrolPointsModeUIClass;

	// pour les infos sur les gardes
	UUserWidget* guardInfo;
	TSubclassOf<UUserWidget> guardInfoUIClass;

public:
		ATyranHUD();
	
		virtual void DrawHUD() override;

		void displayGuardOrder(FVector2D position);

		void removeGuardOrder();

		void displayPatrolPointsMode();

		void removePatrolPointsMode();

		void displayGuardInfo();

		void removeGuardInfo();
};
