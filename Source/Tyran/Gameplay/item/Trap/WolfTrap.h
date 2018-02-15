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
	virtual void triggered() override;
	virtual void Tick(float DeltaTime) override;
	
	
};
