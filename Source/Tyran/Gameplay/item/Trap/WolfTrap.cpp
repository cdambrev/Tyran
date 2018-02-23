// Fill out your copyright notice in the Description page of Project Settings.

#include "WolfTrap.h"
#include "TimerManager.h"



AWolfTrap::AWolfTrap() : Super()
{
	stunTime = 3.0f;
	damage = 25.0f;
}

void AWolfTrap::triggered()
{
	//UE_LOG(LogTemp, Warning,TEXT("WolfTrapTriggered") );
	if (trapTarget && !(trapTarget->isStun)) {
		bIsTriggered = true;
		trapTarget->setTemporarilyStun(stunTime);
		if (PawnOwner)
			trapTarget->TakeDamage(damage, FPointDamageEvent{}, PawnOwner->GetController(), this);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AWolfTrap::triggerDelayedImplementation, stunTime, false);
		//trapTarget = nullptr;
	}
}

void AWolfTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWolfTrap::triggerDelayedImplementation()
{
	Destroy();
}
