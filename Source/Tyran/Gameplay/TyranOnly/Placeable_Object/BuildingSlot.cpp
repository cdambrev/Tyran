// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingSlot.h"
#include <ConstructorHelpers.h>
#include "EngineUtils.h"

// Sets default values
ABuildingSlot::ABuildingSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currBuilding = nullptr;

	static ConstructorHelpers::FClassFinder<ABuilding> defaultDefaultBuilding(TEXT("/Game/Objects/Buildings/2xHouseM1RandomAd"));
	defaultBuilding = defaultDefaultBuilding.Class;
}

void ABuildingSlot::build(TSubclassOf<ABuilding> type)
{
	if (Role == ROLE_Authority) {
		if (currBuilding) {
			currBuilding->Destroy();
		}
		currBuilding = GetWorld()->SpawnActor<ABuilding>(type, GetActorTransform());
	}
	//currBuilding->SetActorTransform(GetActorTransform());
}

// Called when the game starts or when spawned
void ABuildingSlot::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		build(defaultBuilding);
	}
}