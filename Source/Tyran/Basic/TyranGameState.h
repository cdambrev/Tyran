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

protected:
	FTimerHandle endOfGameTimer;

public:
	ATyranGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float gameDuration;

	virtual void BeginPlay() override;
	void OnEndOfGame();

	UFUNCTION(BlueprintCallable)
	FTimespan getTimeLeft();


};
