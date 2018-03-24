// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorLoot.h"
#include "Basic/TyranCharacter.h"



AArmorLoot::AArmorLoot() :Super()
{
	armorReduction = 0.0f;
}

void AArmorLoot::OnUsed(APawn* InstigatorPawn)
{
	ATyranCharacter* charac = Cast<ATyranCharacter>(InstigatorPawn);
	charac->addArmorReduction(armorReduction);
	Destroy();
}
