// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableObjectLoot.h"
#include "GUI/RevHUD.h"
#include <Engine.h>

AUsableObjectLoot::AUsableObjectLoot() {

}

void AUsableObjectLoot::OnUsed(APawn * InstigatorPawn) {
	ATyranCharacter* myPawn = Cast<ATyranCharacter>(InstigatorPawn); 
	
	if (myPawn) {
		if (myPawn->addObjectInInventory(objectClass)) {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("SUCCESS"));
			}
			Destroy();
		}
		else {
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("FAIL"));
			}
		}
	}
}


