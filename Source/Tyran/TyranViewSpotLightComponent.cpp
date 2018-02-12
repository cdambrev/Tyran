// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranViewSpotLightComponent.h"
#include <EngineUtils.h>
#include "TyranController.h"
#include <Engine.h>

UTyranViewSpotLightComponent::UTyranViewSpotLightComponent() {
	angleOfVision = 75.0f;
	cosAoV = cos(PI*(angleOfVision * 2) / 360);
}

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

bool UTyranViewSpotLightComponent::checkVisibility(AActor * actor)
{
	FVector dir = actor->GetActorLocation() - GetComponentTransform().GetLocation();
	dir.Normalize();
	auto forward = GetComponentTransform().GetRotation().GetForwardVector();
	forward.Normalize();
	float cosA = FVector::DotProduct(forward, dir);
	if (cosA > cos(PI*(angleOfVision * 2) / 360)) {
		FCollisionObjectQueryParams objectQueryParams{};
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		FHitResult resultHit{};
		if (GetWorld()->LineTraceSingleByObjectType(resultHit, GetComponentTransform().GetLocation(), actor->GetActorLocation(), objectQueryParams, queryParams)) {
			if (&(*resultHit.Actor) == actor) {
				return true;
			}
		}
	}
	return false;
}

void UTyranViewSpotLightComponent::tryToSee(ATyranCharacter * actor)
{
	if (actor->isAlwaysVisible || checkVisibility(actor)) {
		actor->setViewedThisTick();
	}
}