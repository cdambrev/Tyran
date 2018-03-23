// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableObject.h"

// Sets default values
UUsableObject::UUsableObject()
{
	currentPossess = 1;
}

TSubclassOf<UUsableObject> UUsableObject::getObjectClass() {
	return objectClass;
}

void UUsableObject::setOwner(ATyranCharacter* currCharacter) {
	myPawn = currCharacter;
}