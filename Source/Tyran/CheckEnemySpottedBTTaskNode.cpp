// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySpottedBTTaskNode.h"


EBTNodeResult::Type UCheckEnemySpottedBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UCheckEnemySpottedBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckEnemySpottedBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un ennemi est repere (pas forcement dans le champ de vision)");
}


