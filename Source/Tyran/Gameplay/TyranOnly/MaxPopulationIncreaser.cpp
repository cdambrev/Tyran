// Fill out your copyright notice in the Description page of Project Settings.

#include "MaxPopulationIncreaser.h"
#include "Basic/TyranGameMode.h"
#include "EngineUtils.h"
#include "Basic/ManagerPlayerState.h"


// Sets default values for this component's properties
UMaxPopulationIncreaser::UMaxPopulationIncreaser()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	populationAmount = 0;

	// ...
}


/*
void UMaxPopulationIncreaser::setPopulationAmount_Implementation(int amount)
{
	AManagerPlayerState * playerState = (AManagerPlayerState *)(((ATyranGameMode*)GetWorld()->GetAuthGameMode())->tyranController)->PlayerState;
	playerState->maxPopulation = playerState->maxPopulation - populationAmount + amount;
	populationAmount = amount;
}

bool UMaxPopulationIncreaser::setPopulationAmount_Validate(int amount)
{
	return true;
}*/

// Called when the game starts
void UMaxPopulationIncreaser::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->Role == ROLE_Authority) {
		ATyranController * controller = (((ATyranGameMode*)GetWorld()->GetAuthGameMode())->tyranController);
		if (controller) {
			AManagerPlayerState * playerState = static_cast<AManagerPlayerState *>(controller->PlayerState);
			playerState->maxPopulation = playerState->maxPopulation + populationAmount;
		}
	}
}

void UMaxPopulationIncreaser::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetOwner()->Role == ROLE_Authority) {
		ATyranController * controller = (((ATyranGameMode*)GetWorld()->GetAuthGameMode())->tyranController);
		if (controller) {
			AManagerPlayerState * playerState = static_cast<AManagerPlayerState *>(controller->PlayerState);
			playerState->maxPopulation = playerState->maxPopulation - populationAmount;
		}
	}
}


// Called every frame
void UMaxPopulationIncreaser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

