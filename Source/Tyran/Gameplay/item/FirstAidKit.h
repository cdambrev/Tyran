// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/UsableObject.h"
#include "FirstAidKit.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TYRAN_API FFirstAidKit : public FUsableObject
{
	GENERATED_USTRUCT_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float regeneratedHealth = 20;

public:
	FFirstAidKit();
	
	virtual void onUse() override;
	//virtual TSubclassOf<FUsableObject> getObjectClass() override;
};
