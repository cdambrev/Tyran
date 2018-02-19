// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableObject.h"


// Sets default values
APlaceableObject::APlaceableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	basePrice = 0.0f;

}

// Called when the game starts or when spawned
void APlaceableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



