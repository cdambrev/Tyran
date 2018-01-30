// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranController.h"
#include <EngineUtils.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"

void ATyranController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranController, isTyran);
}

void ATyranController::BeginPlay()
{
	Super::BeginPlay();

}


ATyranController::ATyranController() {
	isTyran = false;

}