// Fill out your copyright notice in the Description page of Project Settings.

#include "RoutineBTTaskNode.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"

EBTNodeResult::Type URoutineBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Routine"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	//Obtenir un pointeur sur AIGuardController
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	//Appeler la fonction UpdateNextTargetPoint qui contient la logique pour selectionner
	// le prochain TargePoint
	AIGuardController->UpdateNextTargetPoint();
	AIGuardController->MoveToLocation(AIGuardController->GetBlackboardComponent()->GetValueAsVector("TargetPointPosition"));
	
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void URoutineBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString URoutineBTTaskNode::GetStaticDescription() const
{
	return TEXT("Routine du garde");
}

