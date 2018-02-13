// Fill out your copyright notice in the Description page of Project Settings.

#include "RechercheBruitBTTaskNode.h"


EBTNodeResult::Type URechercheBruitBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void URechercheBruitBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString URechercheBruitBTTaskNode::GetStaticDescription() const
{
	return TEXT("Rechercher d'ou vient un bruit");
}



