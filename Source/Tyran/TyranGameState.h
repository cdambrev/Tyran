// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TyranCharacter.h"
#include "TyranGameState.generated.h"

/**
 *
 */
UCLASS()
class TYRAN_API ATyranGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATyranGameState();

	/** The Widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
		TSubclassOf<class UUserWidget> HUDWidgetClass;


	/** The instance of the HUD*/
	UPROPERTY()
		class UUserWidget* CurrentWidget;

	/** The duration of the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		FTimespan GameDuration;


	/** Get the time left */
	UFUNCTION(BlueprintPure, Category = "Timer")
		FTimespan GetTimeLeft() const;

	UFUNCTION()
	virtual void BeginPlay() override;
};
