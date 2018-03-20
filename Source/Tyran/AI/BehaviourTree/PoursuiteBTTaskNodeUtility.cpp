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

	// On garde en m�moire que le garde est en poursuite (On est oblig� pour lui demander de s'arr�ter quand il perd le joueur de vue)
	AIGuardController->GetBlackboardComponent()->SetValueAsBool("Poursuit", true);

	// On ex�cute la requ�te permettant au garde de bouger
	EPathFollowingRequestResult::Type resultMove = AIGuardController->MoveToEnemy();

	// S'il est arriv� a destination ou s'il est assez proche avec une ligne de vue pour s'arr�ter et tirer, il s'arr�te et le noeud se termine
	if (resultMove == EPathFollowingRequestResult::AlreadyAtGoal || (checkVisibility(OwnerComp) && dist<distVoulue)) {
		AIGuardController->StopMovement(); // Utile s'il n'est pas encore arriv� � destination mais est suffisamment pres pout s'arr�ter quand m�me.
		NodeResult = EBTNodeResult::Succeeded;
	}
	/*
	// S'il est en mouvement, il reste dans le noeud tant qu'il n'a pas termin� (stu une bonne id�e ?? 
	// Ca veut dire qu'il restera en poursuite meme si l'utility theory determine qu'il y a mieux a faire comme fuir, se mettre a couvert ...)
	else if (resultMove == EPathFollowingRequestResult::RequestSuccessful) {
		NodeResult = EBTNodeResult::InProgress;
	}
	*/
	// S'il est en mouvement, il continue son mouvement et le noeud se termine
	else if (resultMove == EPathFollowingRequestResult::RequestSuccessful) {
		NodeResult = EBTNodeResult::Succeeded;
	}
	// Si la requ�te de mouvement a �chou�, ca fail
	else {
		NodeResult = EBTNodeResult::Failed;
	}
	return NodeResult;
}

void UPoursuiteBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pursue"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	// On ex�cute la requ�te permettant au garde de bouger
	EPathFollowingRequestResult::Type resultMove = AIGuardController->MoveToEnemy();

	// S'il est arriv� a destination ou s'il est assez proche avec une ligne de vue pour s'arr�ter et tirer, il s'arr�te et le noeud se termine
	if (resultMove == EPathFollowingRequestResult::AlreadyAtGoal || (checkVisibility(OwnerComp) && dist < distVoulue)) {
		AIGuardController->StopMovement(); // Utile s'il n'est pas encore arriv� � destination mais est suffisamment pres pout s'arr�ter quand m�me.
		NodeResult = EBTNodeResult::Succeeded;
	}
	// S'il est en mouvement, il continue son mouvement et le noeud se termine
	else if (resultMove == EPathFollowingRequestResult::RequestSuccessful) {
		NodeResult = EBTNodeResult::Succeeded;
	}
	// Si la requ�te de mouvement a �chou�, ca fail
	else {
		NodeResult = EBTNodeResult::Failed;
	}
	return NodeResult;
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
	dist = FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation());
	isVisibleByGuard = checkVisibility(OwnerComp);

	if (!(HeroCharacterActor && guard->modeGuard != ModeGuard::TENIRPOSITION) || (isVisibleByGuard&& dist < distVoulue))
		utility = 0.0f;	// s'il n'a pas de cible, s'il ne peut pas bouger, ou s'il est d�j� bien positionn�, il ne poursuit pas
	else if (!isVisibleByGuard && dist < distVoulue)
		utility = 0.5f; // s'il est assez proche, mais n'a pas la ligne de tir sur la cible, alors plus il est proche, plus il voudra s'approcher pour trouver une ligne de tir
	else {
		utility = 1.0f - distVoulue*0.5f / (dist); // Lorsqu'il est plus loin que la distance voulue, plus il est �loign�, plus il voudra le poursuivre 
	}
	// :S La fonction n'est pas continue
	//1 - distance voulue/ distance a la cible actuelle
	clampUtility();
}


bool UPoursuiteBTTaskNodeUtility::checkVisibility(UBehaviorTreeComponent & OwnerComp) {
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
	FVector dir = HeroCharacterActor->GetActorLocation() - AIGuardController->GetPawn()->GetActorLocation();
	dir.Normalize();
	//float cosA = FVector::DotProduct(AIGuardController->GetPawn()->GetViewRotation()->GetForwardVector(), dir);
	//if (cosA > cos(PI*(angleOfVision * 2) / 360)) {
		FCollisionObjectQueryParams objectQueryParams{};
		FCollisionQueryParams queryParams{};
		queryParams.AddIgnoredActor(AIGuardController->GetPawn());
		FHitResult resultHit{};
		if (GetWorld()->LineTraceSingleByObjectType(resultHit, AIGuardController->GetPawn()->GetActorLocation(), HeroCharacterActor->GetActorLocation(), objectQueryParams, queryParams)) {
			if (&(*resultHit.Actor) == HeroCharacterActor) {
				return true;
			}
		}
	//}
	return false;
}
