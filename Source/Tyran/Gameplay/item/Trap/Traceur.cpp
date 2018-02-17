// Fill out your copyright notice in the Description page of Project Settings.

#include "Traceur.h"

#define COLLISION_WEAPON ECC_GameTraceChannel3



ATraceur::ATraceur() : Super()
{
	
}

void ATraceur::triggered()
{
	if (cible) {
		FString impactResult = cible->GetName();
		UE_LOG(LogTemp, Warning, TEXT("trace touch %s and traceur triggered"), *impactResult);
	}
}

FHitResult ATraceur::TraceurTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	const FName TraceTag("TraceurRay");
	GetWorld()->DebugDrawTraceTag = TraceTag;


	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.TraceTag = TraceTag;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void ATraceur::Tick(float DeltaTime)
{
	Super::Super::Tick(DeltaTime);
	float traceRange = 2000.0f;
	const FVector upVector = GetActorUpVector();
	const FVector StartPos = GetActorLocation() + upVector*2; // + 2 up vector pour que le startPoint ne soit pas dans le mesh
	const FVector EndPos = StartPos + (upVector * traceRange); // Position de fin du tir 
	const FHitResult Impact = TraceurTrace(StartPos, EndPos); // Trouver l'impact 
	if (Impact.GetActor()) {
		if (ATyranCharacter* character = Cast<ATyranCharacter>(Impact.GetActor())) {
			//FString impactResult = character->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("trace touch %s"), *impactResult);
			if (character->getAlignement() != trapOwner) {
				cible = character;
				triggered();
			}
		}
	}

}
