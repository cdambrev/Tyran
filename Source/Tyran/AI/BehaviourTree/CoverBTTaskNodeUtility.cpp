// Fill out your copyright notice in the Description page of Project Settings.

#include "CoverBTTaskNodeUtility.h"

EBTNodeResult::Type UCoverBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Cover"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UCoverBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCoverBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Recherche d'une couverture et positionnement");
}

void UCoverBTTaskNodeUtility::CalculUtility(UBehaviorTreeComponent & OwnerComp)
{
	utility = 0.5f;
}


