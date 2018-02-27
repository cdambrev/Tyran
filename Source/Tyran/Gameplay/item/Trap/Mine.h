// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Trap/Trap.h"
#include "Components/SphereComponent.h"
#include "Mine.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AMine : public ATrap
{
	GENERATED_BODY()

public:
	AMine();

	FPointDamageEvent PointDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float delayBeforeTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* explosionZone;
	UFUNCTION(BlueprintImplementableEvent, Category = "triggerSystem")
	void createExplosion();

	virtual void triggered() override;
	virtual void Tick(float DeltaTime) override;

	virtual FString getType() { return "Mine"; }
private:
	void triggerDelayedImplementation() override;
};
