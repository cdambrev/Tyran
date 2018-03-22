// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoLoot.h"
#include "Basic/TyranCharacter.h"


AAmmoLoot::AAmmoLoot()
{

}

void AAmmoLoot::OnUsed(APawn * InstigatorPawn)
{
	ATyranCharacter* MyPawn = Cast<ATyranCharacter>(InstigatorPawn);

	if (MyPawn) {
		MyPawn->AddAmmo(AmmoType, nbAmmo);
		Destroy();
	}
}
