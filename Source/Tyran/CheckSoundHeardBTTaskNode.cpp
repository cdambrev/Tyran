// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckSoundHeardBTTaskNode.h"

EBTNodeResult::Type UCheckSoundHeardBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void UCheckSoundHeardBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckSoundHeardBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un son suspect a ete entendu");
}


