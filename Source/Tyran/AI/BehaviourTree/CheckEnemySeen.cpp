// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemySeen.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "Basic/GuardCharacter.h"


UCheckEnemySeen::UCheckEnemySeen() {
	Interval = 0.5f;
	bCallTickOnSearchStart = true;
}

void UCheckEnemySeen::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	bool targetIsSeen = checkVisibility(OwnerComp);
	AIGuardController->GetBlackboardComponent()->SetValueAsBool("TargetIsSeen", targetIsSeen);


}


bool UCheckEnemySeen::checkVisibility(UBehaviorTreeComponent & OwnerComp) {
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
	if (!HeroCharacterActor)
	{
		return false;
	}
	FVector dir = HeroCharacterActor->GetActorLocation() - AIGuardController->GetPawn()->GetActorLocation();
	dir.Normalize();
	FCollisionObjectQueryParams objectQueryParams{};
	FCollisionQueryParams queryParams{};
	queryParams.AddIgnoredActor(AIGuardController->GetPawn());
	FHitResult resultHit{};
	if (GetWorld()->LineTraceSingleByObjectType(resultHit, AIGuardController->GetPawn()->GetActorLocation(), HeroCharacterActor->GetActorLocation(), objectQueryParams, queryParams)) {
		if (&(*resultHit.Actor) == HeroCharacterActor) {
			return true;
		}
	}
	return false;
}