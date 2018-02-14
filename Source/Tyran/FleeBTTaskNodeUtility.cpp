// Fill out your copyright notice in the Description page of Project Settings.

#include "FleeBTTaskNodeUtility.h"


EBTNodeResult::Type UFleeBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UFleeBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UFleeBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Si c'était pas assez clair c'est la fuite");
}

void UFleeBTTaskNodeUtility::CalculUtility()
{
	utility = 0.0f;
}

