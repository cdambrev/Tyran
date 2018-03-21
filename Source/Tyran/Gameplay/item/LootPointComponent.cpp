// Fill out your copyright notice in the Description page of Project Settings.

#include "LootPointComponent.h"
#include <EngineUtils.h>
#include "Basic/TyranGameMode.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values for this component's properties
ULootPointComponent::ULootPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	spawnValue = 0;
	spawnZoneRadius = 100.0f;
	// ...
}


// Called when the game starts
void ULootPointComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority) {
		auto lootSet = static_cast<ATyranGameMode *>(GetWorld()->GetAuthGameMode())->availableLoot;
		for (TSubclassOf<ALoot> loot : lootSet) {
			ALoot * defaultLoot = loot.GetDefaultObject();
			float randNum = FMath::RandRange(0.0f, 1.0f);
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			float chanceToSpawn = defaultLoot->maxFrequency * ((1000 - FGenericPlatformMath::Abs(spawnValue - defaultLoot->value)) / 1000);
			auto pos = GetComponentToWorld().GetLocation();
			pos.X += FMath::RandRange(-spawnZoneRadius, spawnZoneRadius);
			pos.Y += FMath::RandRange(-spawnZoneRadius, spawnZoneRadius);
			if (randNum <= chanceToSpawn) {
				GetWorld()->SpawnActor<ALoot>(loot, FTransform(GetComponentToWorld().GetRotation(), pos), params);
			}
		}
	}
}