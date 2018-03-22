// Fill out your copyright notice in the Description page of Project Settings.

#include "RoutineBTTaskNode.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"
#include "Tools/Debug/DebugTools.h"

URoutineBTTaskNode::URoutineBTTaskNode() {
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type URoutineBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Routine"));
#ifdef DEBUG_ON
	Debugger::get().startNodeLog(OwnerComp);
	Debugger::get().addTextLog("Routine ExecuteTask", "ia");
		
#endif
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	//Obtenir un pointeur sur AIGuardController
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());



	//Appeler la fonction UpdateNextTargetPoint qui contient la logique pour selectionner
	// le prochain TargePoint
	FVector targetPointPos = AIGuardController->GetBlackboardComponent()->GetValueAsVector("TargetPointPosition");
	float dist = FVector::Distance(AIGuardController->GetPawn()->GetActorLocation(), targetPointPos);
	if (dist < 200.0f ) {
		AIGuardController->UpdateNextTargetPoint();
	}
	EPathFollowingRequestResult::Type res = AIGuardController->MoveToLocation(targetPointPos, 100.f);

#ifdef DEBUG_ON
	Debugger::get().addArgToNodeLog(OwnerComp, "target", targetPointPos.ToString());
	Debugger::get().addArgToNodeLog(OwnerComp, "distance", FString::FromInt(dist));
	switch (res) {
	case EPathFollowingRequestResult::RequestSuccessful:
		Debugger::get().addArgToNodeLog(OwnerComp, "requete_mouvement", "En mouvement");
		Debugger::get().addTextLog("En mouvement", "routine");
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		Debugger::get().addArgToNodeLog(OwnerComp, "requete_mouvement", "Cible atteinte");
		Debugger::get().addTextLog("Cible atteinte", "routine");
		break;
	case EPathFollowingRequestResult::Failed:
		Debugger::get().addArgToNodeLog(OwnerComp, "requete_mouvement", "La requete de mouvement a echoue");
		Debugger::get().addTextLog("La requete de mouvement a echoue", "routine");
		break;
	}
#endif
	if (res == EPathFollowingRequestResult::RequestSuccessful || res == EPathFollowingRequestResult::AlreadyAtGoal)
		NodeResult = EBTNodeResult::Succeeded;
	else
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

void URoutineBTTaskNode::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult) {
#ifdef DEBUG_ON
	Debugger::get().endNodeLog(OwnerComp);
#endif
}

