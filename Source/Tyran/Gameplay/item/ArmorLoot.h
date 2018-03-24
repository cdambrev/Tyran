// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Loot.h"
#include "ArmorLoot.generated.h"

/**
 * 
 */
UCLASS(abstract)
class TYRAN_API AArmorLoot : public ALoot
{
	GENERATED_BODY()

protected :
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float armorReduction;
public:
	AArmorLoot();
	
	virtual void OnUsed(APawn* InstigatorPawn) override;
	
};
