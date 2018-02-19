// Fill out your copyright notice in the Description page of Project Settings.

#include "Traceur.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#define COLLISION_WEAPON ECC_GameTraceChannel3



ATraceur::ATraceur() : Super()
{
	visibilityDelay = 2.0f;
	coolDown = 1.0f;
	beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));
	beam->SetupAttachment(RootComponent);
	beam->SetBeamSourcePoint(0, GetActorLocation(), 0);
	beam->SetBeamTargetPoint(0, GetActorLocation(), 0);

	
}

void ATraceur::triggered()
{
	if (trapTarget && !(trapTarget->isTraced)) {
		bIsTriggered = true;
		//FTimerHandle UnusedHandle;
		//FString impactResult = cible->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("trace touch %s and traceur triggered"), *impactResult);
		trapTarget->setTemporarilyVisible(visibilityDelay);
		FTimerHandle UnusedHandle2;
		GetWorldTimerManager().SetTimer(UnusedHandle2, this, &ATraceur::TraceurCoolDown, coolDown, false);
		trapTarget = nullptr;
	}
}




void ATraceur::TraceurCoolDown()
{
	bIsTriggered = false;
}

FHitResult ATraceur::TraceurTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	//const FName TraceTag("TraceurRay");
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	//TraceParams.TraceTag = TraceTag;
	TraceParams.AddIgnoredActor(this); //tres Important
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void ATraceur::Tick(float DeltaTime)
{
	Super::Super::Tick(DeltaTime);
	float traceRange = 2000.0f;
	const FVector upVector = GetActorUpVector();
	const FVector StartPos = GetActorLocation();
	const FVector EndPos = StartPos + (upVector * traceRange); // Position de fin du tir 
	const FHitResult Impact = TraceurTrace(StartPos, EndPos); // Trouver l'impact 
	if (Impact.GetActor()) {
		//FString impactResult = Impact.GetActor()->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("trace touch %s"), *impactResult);
		beam->SetBeamTargetPoint(0, Impact.ImpactPoint, 0);
		if (ATyranCharacter* character = Cast<ATyranCharacter>(Impact.GetActor())) {
			//FString impactResult = character->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("trace touch %s"), *impactResult);
			if (character->getAlignement() != trapOwner && !bIsTriggered) {
				trapTarget = character;
				triggered();
			}
		}
	}
	else {
		beam->SetBeamTargetPoint(0, StartPos, 0);
	}

}
