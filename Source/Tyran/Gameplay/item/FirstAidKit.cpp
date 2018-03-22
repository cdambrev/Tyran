// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstAidKit.h"


FFirstAidKit::FFirstAidKit() {
	stackPossible = 5;
	//objectClass = this->getObjectClass();
}

void FFirstAidKit::onUse() {
	myPawn->regenerate(regeneratedHealth);
	currentPossess--;
}

//TSubclassOf<FUsableObject> FFirstAidKit::getObjectClass() {
//	//return objectClass;
}
