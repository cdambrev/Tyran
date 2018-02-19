// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Trap/Trap.h"
#include "WolfTrap.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AWolfTrap : public ATrap
{
	GENERATED_BODY()

public:

	AWolfTrap();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stunTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage;
	virtual void triggered() override;
	virtual void Tick(float DeltaTime) override;
	virtual FString getType() { return "WolfTrap"; }

protected:
	virtual void triggerDelayedImplementation();
};
