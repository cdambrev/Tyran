// Fill out your copyright notice in the Description page of Project Settings.

#include "ChoixEnnemiBTTaskNode.h"


EBTNodeResult::Type UChoixEnnemiBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("EnemyChoice"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UChoixEnnemiBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UChoixEnnemiBTTaskNode::GetStaticDescription() const
{
	return TEXT("Choisi quel ennemi prendre en focus pour la prochaine action");
}


