// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranGameState.h"
#include <EngineUtils.h>
#include "TyranController.h"
#include <TimerManager.h>
#include <Engine/World.h>

ATyranGameState::ATyranGameState() {
	gameDuration = FTimespan{ 0,5,0 }.GetTotalSeconds();
}

void ATyranGameState::BeginPlay() {
	GetWorldTimerManager().SetTimer(endOfGameTimer, this, &ATyranGameState::OnEndOfGame, gameDuration, false, -1.f);
}



void ATyranGameState::OnEndOfGame() {
	for (TActorIterator<ATyranController> It(GetWorld()); It; ++It) {
		It->EndOfGame();
	}
}

FTimespan ATyranGameState::getTimeLeft() {
	return FTimespan{ 0, 0, static_cast<int32>(GetWorldTimerManager().GetTimerRemaining(endOfGameTimer)) };
}
