// Fill out your copyright notice in the Description page of Project Settings.

#include "Mine.h"
#include "Array.h"
#include "Basic/TyranCharacter.h"




AMine::AMine() : Super()
{
	//PointDmg.DamageTypeClass = {};
	//PointDmg.HitInfo = Impact;
	//PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = 50000;

	explosionZone = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionZone"));
	explosionZone->SetupAttachment(myMesh);

}



void AMine::triggered()
{
	isTriggered = true;
	createExplosion();
	TArray<AActor*> OverlappingActors;
	explosionZone->GetOverlappingActors(OverlappingActors);
	for (auto& i : OverlappingActors) {

		if (ATyranCharacter* character = Cast<ATyranCharacter>(i)) {
			FString f = character->GetName();
			UE_LOG(LogTemp, Warning,TEXT("character %s get touch by explosion"),*f );

			auto a = PointDmg;
			auto b =PawnOwner->GetController();
			
			//character->TakeDamage(50000, PointDmg, PawnOwner->GetController(),this);
		}
	}
	Destroy();
}

void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
