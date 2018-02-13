// Fill out your copyright notice in the Description page of Project Settings.

#include "PoursuiteBTTaskNodeUtility.h"


EBTNodeResult::Type UPoursuiteBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UPoursuiteBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UPoursuiteBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Poursuivre sa cible");
}

