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
	UUserWidget* guardOrderWidget;
	TSubclassOf<UUserWidget> guardUIClass;

public:
		ATyranHUD();
	
		virtual void DrawHUD() override;

		void displayGuardOrder(FVector2D position);

		void removeGuardOrder();
};
