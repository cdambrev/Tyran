// Fill out your copyright notice in the Description page of Project Settings.

#include "RessourceProducer.h"
#include "Basic/TyranGameMode.h"
#include "EngineUtils.h"
#include "Basic/ManagerPlayerState.h"


// Sets default values for this component's properties
URessourceProducer::URessourceProducer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	resourseProduction = 0.0f;
	// ...
}


// Called when the game starts
void URessourceProducer::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->Role == ROLE_Authority) {
		ATyranController * controller = (((ATyranGameMode*)GetWorld()->GetAuthGameMode())->tyranController);
		if (controller) {
			AManagerPlayerState * playerState = static_cast<AManagerPlayerState *>(controller->PlayerState);
			playerState->addMoneyIncome(resourseProduction);
		}
	}
}

void URessourceProducer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (GetOwner()->Role == ROLE_Authority) {
		ATyranController * controller = (((ATyranGameMode*)GetWorld()->GetAuthGameMode())->tyranController);
		if (controller) {
			AManagerPlayerState * playerState = static_cast<AManagerPlayerState *>(controller->PlayerState);
			playerState->addMoneyIncome(-resourseProduction);
		}
	}
}


// Called every frame
void URessourceProducer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

