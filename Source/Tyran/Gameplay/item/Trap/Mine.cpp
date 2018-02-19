// Fill out your copyright notice in the Description page of Project Settings.

#include "Mine.h"
#include "Array.h"
#include "Basic/TyranCharacter.h"
#include "TimerManager.h"




AMine::AMine() : Super()
{
	//PointDmg.DamageTypeClass = {};
	//PointDmg.HitInfo = Impact;
	//PointDmg.ShotDirection = ShootDir;
	//PointDmg.Damage = 50000;
	delayBeforeTrigger = 0.5f;
	basePrice = 5.0f;
	explosionZone = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionZone"));
	explosionZone->SetupAttachment(myMesh);

}



void AMine::triggered()
{
	bIsTriggered = true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMine::triggerDelayedImplementation, delayBeforeTrigger, false);

	
}
 
void AMine::triggerDelayedImplementation()
{
	createExplosion();
	TArray<AActor*> OverlappingActors;
	explosionZone->GetOverlappingActors(OverlappingActors);
	for (auto& i : OverlappingActors) {

		if (ATyranCharacter* character = Cast<ATyranCharacter>(i)) {
			//FString f = character->GetName();
			//UE_LOG(LogTemp, Warning,TEXT("character %s get touch by explosion"),*f );
			if (PawnOwner)
				character->TakeDamage(50, PointDmg, PawnOwner->GetController(), this);
		}
		else if (AMine* mine = Cast<AMine>(i)) {
			if (!(mine->bIsTriggered))
				mine->triggered();
		}
	}
	Destroy();
}


void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

