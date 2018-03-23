// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstAidKit.h"
#include <Engine.h>

UFirstAidKit::UFirstAidKit() {
	stackPossible = 5;
	objectClass = UFirstAidKit::StaticClass();
}

void UFirstAidKit::onUse() {
	myPawn->regenerate(regeneratedHealth);
	currentPossess--;
}

bool UFirstAidKit::add() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Stack possible : ") + FString::FromInt(stackPossible) + TEXT("Current possess : ") + FString::FromInt(currentPossess));
	}
	if (currentPossess < stackPossible) {
		currentPossess++;
		return true;
	}
	return false;
}