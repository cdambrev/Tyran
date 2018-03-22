// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"


AGuardCharacter::AGuardCharacter() {
	alignement = EAlignement::A_TYRAN;
	cost = 0.0f;
	populationCost = 1;
}

void AGuardCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGuardCharacter, modeGuard);
	DOREPLIFETIME(AGuardCharacter, fuiteAutorise);
}
