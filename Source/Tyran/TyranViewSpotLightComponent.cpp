// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranViewSpotLightComponent.h"
#include <EngineUtils.h>
#include "TyranController.h"
#include <Engine.h>



void UTyranViewSpotLightComponent::BeginPlay()
{
	bool tyran = false;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		if (static_cast<ATyranController *>(&**Iterator)->IsLocalPlayerController() && static_cast<ATyranController *>(&**Iterator)->isTyran) {
			tyran = true;
		}
	}
	if (!tyran) {
		SetVisibility(false);
	}
}