// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardSpawnPoint.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"


// Sets default values for this component's properties
UGuardSpawnPoint::UGuardSpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGuardSpawnPoint::spawnGuard(TSubclassOf<AGuardCharacter> gClass)
{
	auto guard = GetWorld()->SpawnActor<AGuardCharacter>(gClass, FTransform{ UNavigationSystem::ProjectPointToNavigation(GetWorld(),GetComponentToWorld().GetLocation()) });
}
