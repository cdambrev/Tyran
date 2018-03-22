// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySpottedBTTaskNode.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AI/AIGuardController.h"

EBTNodeResult::Type UCheckEnemySpottedBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("CheckEnemySpotted"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));

	if (HeroCharacterActor){
		NodeResult = EBTNodeResult::Succeeded;
	}
	else {
		NodeResult = EBTNodeResult::Failed;
	}
	return NodeResult;
}

void UCheckEnemySpottedBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UCheckEnemySpottedBTTaskNode::GetStaticDescription() const
{
	return TEXT("Verifie si un ennemi est repere (pas forcement dans le champ de vision)");
}


