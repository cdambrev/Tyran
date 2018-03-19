// Fill out your copyright notice in the Description page of Project Settings.

#include "PoursuiteBTTaskNodeUtility.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "Basic/GuardCharacter.h"

EBTNodeResult::Type UPoursuiteBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pursue"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	if (AIGuardController->MoveToEnemy() == EPathFollowingRequestResult::RequestSuccessful)
		NodeResult = EBTNodeResult::Succeeded;
	else
		NodeResult = EBTNodeResult::Failed;

	return NodeResult;
}

void UPoursuiteBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UPoursuiteBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Poursuivre sa cible");
}

void UPoursuiteBTTaskNodeUtility::CalculUtility(UBehaviorTreeComponent & OwnerComp)
{
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
	AGuardCharacter * guard = Cast<AGuardCharacter>(AIGuardController->GetPawn());
	if (HeroCharacterActor && guard->modeGuard != ModeGuard::TENIRPOSITION )
		utility = 1.0f - 500/(FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation()));
	else
		utility = 0.0f;
	//1 - distance voulue/ distance a la cible actuelle

	clampUtility();
}


