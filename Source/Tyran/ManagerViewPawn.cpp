// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerViewPawn.h"


// Sets default values
AManagerViewPawn::AManagerViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManagerViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AManagerViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AManagerViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

