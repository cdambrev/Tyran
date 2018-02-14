// Fill out your copyright notice in the Description page of Project Settings.

#include "Loot.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALoot::ALoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	value = 0;
	maxFrequency = 0;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ALoot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

