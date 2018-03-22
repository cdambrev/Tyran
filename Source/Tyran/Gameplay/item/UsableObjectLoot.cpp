// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableObjectLoot.h"

AUsableObjectLoot::AUsableObjectLoot() {

}

void AUsableObjectLoot::OnUsed(APawn * InstigatorPawn) {
	ATyranCharacter* myPawn = Cast<ATyranCharacter>(InstigatorPawn); 
	
	if (myPawn) {
	//	myPawn->addObject

		Destroy();
	}
}


