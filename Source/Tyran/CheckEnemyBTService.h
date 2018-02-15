// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckEnemyBTService.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UCheckEnemyBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckEnemyBTService();

	//Sera appele a chaque update du service
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
	
	
};
