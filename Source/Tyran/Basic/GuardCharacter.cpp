// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"


AGuardCharacter::AGuardCharacter() {
	
}

void AGuardCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGuardCharacter, modeGuard);
	DOREPLIFETIME(AGuardCharacter, fuiteAutorise);
}
