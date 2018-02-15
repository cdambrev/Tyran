// Fill out your copyright notice in the Description page of Project Settings.

#include "Trap.h"


// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	trapOwner = EAlignement::A_TYRAN;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap"));
	myMesh->SetupAttachment(RootComponent);
	placementCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementCollision"));
	placementCollision->SetupAttachment(myMesh);
	triggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	triggerZone->SetupAttachment(myMesh);

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

