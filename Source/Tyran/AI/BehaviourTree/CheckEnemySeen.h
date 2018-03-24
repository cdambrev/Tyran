// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckEnemySeen.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UCheckEnemySeen : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckEnemySeen();
	void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;
protected:
	bool checkVisibility(UBehaviorTreeComponent & OwnerComp);
	
};
