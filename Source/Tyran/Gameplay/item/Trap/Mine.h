// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Trap/Trap.h"
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
	UFUNCTION(BlueprintImplementableEvent, Category = "triggerSystem")
	void createExplosion();

	virtual void triggered() override;
	virtual void Tick(float DeltaTime) override;

	
};
