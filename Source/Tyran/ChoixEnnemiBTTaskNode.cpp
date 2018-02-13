// Fill out your copyright notice in the Description page of Project Settings.

#include "ChoixEnnemiBTTaskNode.h"


EBTNodeResult::Type UChoixEnnemiBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UChoixEnnemiBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UChoixEnnemiBTTaskNode::GetStaticDescription() const
{
	return TEXT("Choisi quel ennemi prendre en focus pour la prochaine action");
}


