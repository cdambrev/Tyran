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

void UFirstAidKit::add() {
	if (currentPossess < stackPossible) {
		currentPossess++;
	}
}