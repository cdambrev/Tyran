// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckActionZoneBTTaskNode.h"


EBTNodeResult::Type UCheckActionZoneBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("CheckActionZone"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UCheckActionZoneBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckActionZoneBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si le guarde se trouve dans la zone ou il peut agir seul");
}

