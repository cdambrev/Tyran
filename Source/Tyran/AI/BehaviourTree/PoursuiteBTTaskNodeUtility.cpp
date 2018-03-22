// Fill out your copyright notice in the Description page of Project Settings.

#include "PoursuiteBTTaskNodeUtility.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "Basic/GuardCharacter.h"
#include "Tools/Debug/DebugTools.h"

//#define POURSUITE_DEBUG

UPoursuiteBTTaskNodeUtility::UPoursuiteBTTaskNodeUtility() {
	// Le nom que prendra le noeud dans le BT 
	//NodeName = "Poursuite";
	// Cette tâche appelle TickTask 
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPoursuiteBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
#ifdef DEBUG_ON
	Debugger::get().addTextLog("Poursuite ExecuteTask", "ia");
#endif
	//UE_LOG(LogTemp, Warning, TEXT("Pursue"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner()); 
	AGuardCharacter * Guard = Cast<AGuardCharacter>(AIGuardController->GetPawn());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));

	if (!HeroCharacterActor || Guard->modeGuard == ModeGuard::TENIRPOSITION)
	{
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
		Debugger::get().addTextLog("Pas de cible.", "poursuite");
#endif
#endif
		NodeResult = EBTNodeResult::Succeeded;
	}
	else {
		// On exécute la requête permettant au garde de bouger
		EPathFollowingRequestResult::Type resultMove = AIGuardController->MoveToEnemy();

		// S'il est arrivé a destination ou s'il est assez proche avec une ligne de vue pour s'arrêter et tirer, il s'arrête et le noeud se termine
		if (resultMove == EPathFollowingRequestResult::AlreadyAtGoal || (checkVisibility(OwnerComp) && dist < distVoulue)) {
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
			if (resultMove == EPathFollowingRequestResult::AlreadyAtGoal)
				Debugger::get().addTextLog("Cible atteinte. Retour Success", "poursuite");
			else
				Debugger::get().addTextLog("Cible en vue. Retour Success", "poursuite");
#endif
#endif 
			NodeResult = EBTNodeResult::Succeeded;
		}
		/*
		// S'il est en mouvement, il reste dans le noeud tant qu'il n'a pas terminé (stu une bonne idée ??
		// Ca veut dire qu'il restera en poursuite meme si l'utility theory determine qu'il y a mieux a faire comme fuir, se mettre a couvert ...)
		else if (resultMove == EPathFollowingRequestResult::RequestSuccessful) {
			NodeResult = EBTNodeResult::InProgress;
		}
		*/
		// S'il est en mouvement, il reste dans le noeud tant qu'il n'a pas terminé
		else if (resultMove == EPathFollowingRequestResult::RequestSuccessful) {
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
			Debugger::get().addTextLog("En mouvement. Retour inProgress", "poursuite");
#endif
#endif
			NodeResult = EBTNodeResult::InProgress;
#ifdef DEBUG_ON	
			Debugger::get().startNodeLog(OwnerComp);
#endif

		}
		// Si la requête de mouvement a échoué, ca fail
		else {
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
			Debugger::get().addTextLog("Requete de mouvement echouee", "poursuite");
#endif
#endif
			NodeResult = EBTNodeResult::Succeeded;
		}
	}
	return NodeResult;
}

void UPoursuiteBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
#ifdef DEBUG_ON 
#ifdef POURSUITE_DEBUG
	Debugger::get().addTextLog("Poursuite TickTask", "ia");
#endif
#endif
	//UE_LOG(LogTemp, Warning, TEXT("Pursue"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AGuardCharacter * Guard = Cast<AGuardCharacter>(AIGuardController->GetPawn());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));

	if (!HeroCharacterActor || Guard->modeGuard == ModeGuard::TENIRPOSITION)
	{
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
		Debugger::get().addTextLog("Pas de cible.", "poursuite");
#endif
#endif
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else {
		// On exécute la requête permettant au garde de bouger
		EPathFollowingRequestResult::Type resultMove = AIGuardController->MoveToEnemy();

		dist = FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation());
		isVisibleByGuard = checkVisibility(OwnerComp);

		// S'il est arrivé a destination ou s'il est assez proche avec une ligne de vue pour s'arrêter et tirer, il s'arrête et le noeud se termine
		if (resultMove == EPathFollowingRequestResult::AlreadyAtGoal || (isVisibleByGuard && dist < distVoulue)) {
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
			if (isVisibleByGuard)
			{
				Debugger::get().addTextLog("Cible en vue. Retour Success", "poursuite");
			}
			else {
				Debugger::get().addTextLog("Cible atteinte. Retour Success", "poursuite");
		}
#endif
#endif
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		// Si la requête de mouvement a échoué, ca fail
		else if (resultMove == EPathFollowingRequestResult::Failed) {
#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
			Debugger::get().addTextLog("Requete de mouvement echouee", "poursuite");
#endif
#endif
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
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
	if (!HeroCharacterActor)
	{
		utility = 0.0f;
	}
	else {
		dist = FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation());
		isVisibleByGuard = checkVisibility(OwnerComp);

		if (guard->modeGuard == ModeGuard::TENIRPOSITION || (isVisibleByGuard && dist < distVoulue))
			// s'il n'a pas de cible, s'il ne peut pas bouger, ou s'il est déjà bien positionné, il ne poursuit pas
			utility = 0.f;
		else if (!isVisibleByGuard && dist < distVoulue)
			utility = 0.5f; // s'il est assez proche, mais n'a pas la ligne de tir sur la cible, alors il voudra s'approcher pour trouver une ligne de tir
		else {
			utility = 1.0f - distVoulue * 0.5f / (dist); // Lorsqu'il est plus loin que la distance voulue, plus il est éloigné, plus il voudra le poursuivre 
			//1 - distance voulue/ distance a la cible actuelle
		}
	}
	clampUtility();
}

void UPoursuiteBTTaskNodeUtility::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AIGuardController->StopMovement();

#ifdef DEBUG_ON
#ifdef POURSUITE_DEBUG
	Debugger::get().addTextLog("Fin du noeud", "poursuite");
#endif
	Debugger::get().endNodeLog(OwnerComp);
#endif
}


bool UPoursuiteBTTaskNodeUtility::checkVisibility(UBehaviorTreeComponent & OwnerComp) {
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
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
