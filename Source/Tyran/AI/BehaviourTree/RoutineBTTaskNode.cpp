// Fill out your copyright notice in the Description page of Project Settings.

#include "RoutineBTTaskNode.h"


EBTNodeResult::Type URoutineBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Routine"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void URoutineBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString URoutineBTTaskNode::GetStaticDescription() const
{
	return TEXT("Routine du garde");
}

