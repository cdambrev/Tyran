// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranGameState.h"
#include <EngineUtils.h>
#include "TyranController.h"
#include <TimerManager.h>
#include <Engine/World.h>

void ATyranGameState::BeginPlay() {
	GetWorldTimerManager().SetTimer(endOfGameTimer, this, &ATyranGameState::OnEndOfGame, 10.f, false, -1.f);
}



void ATyranGameState::OnEndOfGame() {
	for (TActorIterator<ATyranController> It(GetWorld()); It; ++It) {
		It->EndOfGame();
	}
}
