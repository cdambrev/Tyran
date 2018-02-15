// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySeenBTTaskNode.h"

EBTNodeResult::Type UCheckEnemySeenBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("CheckEnemySeen"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UCheckEnemySeenBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckEnemySeenBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un ennemi est dans le champ de vision");
}



