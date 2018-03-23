// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/UsableObject.h"
#include "FirstAidKit.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TYRAN_API UFirstAidKit : public UUsableObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float regeneratedHealth = 20;

public:
	UFirstAidKit();
	
	virtual void onUse() override;
	virtual bool add() override;
};
