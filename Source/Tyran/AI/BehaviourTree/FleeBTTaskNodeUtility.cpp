// Fill out your copyright notice in the Description page of Project Settings.

#include "FleeBTTaskNodeUtility.h"


EBTNodeResult::Type UFleeBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Flee"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UFleeBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UFleeBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("C'est la fuite");
}

void UFleeBTTaskNodeUtility::CalculUtility(UBehaviorTreeComponent & OwnerComp)
{
	utility = 0.0f;
	clampUtility();
}
