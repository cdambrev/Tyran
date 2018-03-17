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
	FTimerHandle endOfGameTimer;

	virtual void BeginPlay() override;
	void OnEndOfGame();
};
