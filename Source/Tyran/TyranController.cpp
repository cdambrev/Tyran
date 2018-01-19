// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranController.h"

void ATyranController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranController, isTyran);
}


ATyranController::ATyranController() {
	isTyran = false;
}