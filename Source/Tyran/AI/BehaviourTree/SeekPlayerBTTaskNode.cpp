// Fill out your copyright notice in the Description page of Project Settings.

#include "SeekPlayerBTTaskNode.h"

EBTNodeResult::Type USeekPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("SeekPlayer"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Failed;
	return NodeResult;
}

void USeekPlayerBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString USeekPlayerBTTaskNode::GetStaticDescription() const
{
	return TEXT("Recherche d'un joueur qui s'est enfui");
}


