// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySpottedBTTaskNode.h"


EBTNodeResult::Type UCheckEnemySpottedBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("CheckEnemySpotted"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UCheckEnemySpottedBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckEnemySpottedBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un ennemi est repere (pas forcement dans le champ de vision)");
}


