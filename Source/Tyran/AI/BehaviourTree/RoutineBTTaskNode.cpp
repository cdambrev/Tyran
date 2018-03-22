// Fill out your copyright notice in the Description page of Project Settings.

#include "RoutineBTTaskNode.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"
#include "Tools/Debug/DebugTools.h"
#include "Basic/GuardCharacter.h"

EBTNodeResult::Type URoutineBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Routine"));
	
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	//Obtenir un pointeur sur AIGuardController
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	

	//Appeler la fonction UpdateNextTargetPoint qui contient la logique pour selectionner
	// le prochain TargePoint
	if (FVector::Distance(AIGuardController->GetPawn()->GetActorLocation(), AIGuardController->GetBlackboardComponent()->GetValueAsVector("TargetPointPosition")) < 200.0 ) {
		AIGuardController->UpdateNextTargetPoint();
	}
	AIGuardController->MoveToLocation(AIGuardController->GetBlackboardComponent()->GetValueAsVector("TargetPointPosition"));
	
	NodeResult = EBTNodeResult::Succeeded;

#ifdef DEBUG_ON
	if (NodeResult != EBTNodeResult::Failed) {
		Debugger::get().startNodeLog(OwnerComp);
		Debugger::get().addArgToNodeLog(OwnerComp, "test", "test");
	}
		
	if(NodeResult == EBTNodeResult::Succeeded)
		Debugger::get().endNodeLog(OwnerComp);
#endif
	if (NodeResult == EBTNodeResult::Succeeded) {
 		if(AIGuardController->isFighting())
 			AIGuardController->exitFight();
	}
	return NodeResult;
}

void URoutineBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString URoutineBTTaskNode::GetStaticDescription() const
{
	return TEXT("Routine du garde");
}

void URoutineBTTaskNode::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult) {
#ifdef DEBUG_ON
	Debugger::get().endNodeLog(OwnerComp);
#endif
}

