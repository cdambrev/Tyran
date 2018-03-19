// Fill out your copyright notice in the Description page of Project Settings.

#include "PoursuiteBTTaskNodeUtility.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 

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

	if (HeroCharacterActor)
		utility = 1.0f - 500.f/(FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation()));
	else
		utility = 0.0f;
	//1 - distance voulue/ distance a la cible actuelle

	clampUtility();
}


bool UPoursuiteBTTaskNodeUtility::checkVisibility(AActor * actor) {
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	FVector dir = actor->GetActorLocation() - GetComponentTransform().GetLocation();
	dir.Normalize();
	float cosA = FVector::DotProduct(GetComponentTransform().GetRotation().GetForwardVector(), dir);
	if (cosA > cos(PI*(angleOfVision * 2) / 360)) {
		FCollisionObjectQueryParams objectQueryParams{};
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(GetOwner());
		FHitResult resultHit{};
		if (GetWorld()->LineTraceSingleByObjectType(resultHit, GetComponentTransform().GetLocation(), actor->GetActorLocation(), objectQueryParams, queryParams)) {
			if (&(*resultHit.Actor) == actor) {
				return true;
			}
		}
	}
	return false;
}
