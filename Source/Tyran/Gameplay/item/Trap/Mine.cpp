// Fill out your copyright notice in the Description page of Project Settings.

#include "Mine.h"
#include "Array.h"
#include "Basic/TyranCharacter.h"




AMine::AMine() : Super()
{

}



void AMine::triggered()
{
	UE_LOG(LogTemp, Warning, TEXT("Triggered Mine"));
	createExplosion();
	Destroy();

}

void AMine::Tick(float DeltaTime)
{
	TArray<AActor*> OverlappingActors;
	triggerZone->GetOverlappingActors(OverlappingActors);
	for (auto& i : OverlappingActors) {
		if (ATyranCharacter* character = Cast<ATyranCharacter>(i)) {
			if (character->getAlignement() != trapOwner)
			{
				triggered();
			}
		}
	}
}
