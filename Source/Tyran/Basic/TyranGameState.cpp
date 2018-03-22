// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranGameState.h"
#include <EngineUtils.h>
#include "TyranController.h"
#include <TimerManager.h>
#include <Engine/World.h>

ATyranGameState::ATyranGameState() {
	gameDuration = FTimespan{ 0,15,0 }.GetTotalSeconds();
}

void ATyranGameState::BeginPlay() {
	GetWorldTimerManager().SetTimer(endOfGameTimer, this, &ATyranGameState::TyranWin, gameDuration, false, -1.f);
}



void ATyranGameState::TyranWin() {
	GetWorldTimerManager().ClearTimer(endOfGameTimer);
	for (TActorIterator<ATyranController> It(GetWorld()); It; ++It) {
		It->ClientRPCTyranWin();
	}
}

void ATyranGameState::RevWin() {
	UE_LOG(LogTemp, Warning, TEXT("RevWin"));
	GetWorldTimerManager().ClearTimer(endOfGameTimer);
	for (TActorIterator<ATyranController> It(GetWorld()); It; ++It) {
		It->ClientRPCRevWin();
	}
}

/*
void ATyranGameState::EndGame(bool revWin) {
	//GetWorldTimerManager().ClearTimer(endOfGameTimer);
	if (revWin)
		RevWin();
	TyranWin();
}*/



FTimespan ATyranGameState::getTimeLeft() {
	return FTimespan{ 0, 0, static_cast<int32>(GetWorldTimerManager().GetTimerRemaining(endOfGameTimer)) };
}
