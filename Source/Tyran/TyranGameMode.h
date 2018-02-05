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

	virtual void BeginPlay() override;

protected:
	/** The Widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
		TSubclassOf<class UUserWidget> HUDWidgetClass;


	/** The instance of the HUD*/
	UPROPERTY()
		class UUserWidget* CurrentWidget;

	/** The duration of the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer")
		FTimespan GameDuration;

	/** The time left */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timer")
		FTimespan TimeLeft;
public:

	/** Get the time left */
	UFUNCTION(BlueprintPure, Category = "Timer")
		FTimespan GetTimeLeft() const;

	/** Update the time left */
	UFUNCTION(BlueprintCallable, Category = "Timer")
		void UpdateTimeLeft(const FTimespan& deltat);

	/** Init the time left */
	UFUNCTION(BlueprintCallable, Category = "Timer")
		void InitTimeLeft(const FTimespan& init);
};



