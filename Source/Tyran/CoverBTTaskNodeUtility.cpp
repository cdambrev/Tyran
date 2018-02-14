// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverBTTaskNodeUtility.h"

EBTNodeResult::Type UCoverBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UCoverBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCoverBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Recherche d'une couverture et positionnement");
}

void UCoverBTTaskNodeUtility::CalculUtility()
{
	utility = 0.0f;
}
