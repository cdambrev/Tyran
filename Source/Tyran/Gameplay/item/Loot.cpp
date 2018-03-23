// Fill out your copyright notice in the Description page of Project Settings.

#include "Loot.h"

#include "Components/StaticMeshComponent.h"
#include "Gameplay/Interaction/LootInteractionComponent.h"


// Sets default values
ALoot::ALoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	value = 0;
	maxFrequency = 0;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	RootComponent = MeshComp;

	lootInteraction = CreateDefaultSubobject<ULootInteractionComponent>(TEXT("loot"));
	lootInteraction->SetupAttachment(RootComponent);
	bReplicates = true; 
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ALoot::BeginPlay()
{
	Super::BeginPlay();
	
	// Tous les objets qui peuvent �tre ramass�s sont simul�s 
	// et peuvent �tre diss�min�s sur tout le niveau
	MeshComp->SetSimulatePhysics(true);
	MeshComp->WakeRigidBody();
}

// Called every frame
void ALoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

