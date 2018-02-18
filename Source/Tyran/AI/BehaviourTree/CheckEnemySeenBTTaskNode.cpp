// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySeenBTTaskNode.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"

EBTNodeResult::Type UCheckEnemySeenBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("CheckEnemySeen"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));

	if (HeroCharacterActor)
		NodeResult = EBTNodeResult::Succeeded;
	else
		NodeResult = EBTNodeResult::Failed;

	return NodeResult;
}

void UCheckEnemySeenBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckEnemySeenBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un ennemi est dans le champ de vision");
}


