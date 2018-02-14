// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootBTTaskNodeUtility.h"


EBTNodeResult::Type UShootBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Shoot"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UShootBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UShootBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Vise et tire sur un joueur");
}

void UShootBTTaskNodeUtility::CalculUtility()
{
	utility = 0.0f;
}


