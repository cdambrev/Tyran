// Fill out your copyright notice in the Description page of Project Settings.

#include "LootPoint.h"
#include <EngineUtils.h>
#include "TyranGameMode.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values
ALootPoint::ALootPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	spawnValue = 0;
}

// Called when the game starts or when spawned
void ALootPoint::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		auto lootSet = static_cast<ATyranGameMode *>(GetWorld()->GetAuthGameMode())->availableLoot;
		for (TSubclassOf<ALoot> loot : lootSet) {
			ALoot * defaultLoot = loot.GetDefaultObject();
			float randNum = FMath::RandRange(0.0f, 1.0f);
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			float chanceToSpawn = defaultLoot->maxFrequency * ((1000 - FGenericPlatformMath::Abs(spawnValue - defaultLoot->value))/1000);
			if (randNum <= chanceToSpawn) {
				GetWorld()->SpawnActor<ALoot>(loot, GetActorTransform(),params);
			}
		}
	}
}

// Called every frame
void ALootPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

