// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstAidKit.h"
#include <Engine.h>

UFirstAidKit::UFirstAidKit() {
	stackPossible = 5;
	objectClass = UFirstAidKit::StaticClass();
}

void UFirstAidKit::onUse() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ca regen :)"));
	}
	myPawn->regenerate(regeneratedHealth);
	currentPossess--;
}

void UFirstAidKit::add() {
	if (currentPossess < stackPossible) {
		currentPossess++;
	}
}