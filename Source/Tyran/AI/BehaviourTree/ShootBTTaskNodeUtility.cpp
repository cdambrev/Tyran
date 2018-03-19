// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootBTTaskNodeUtility.h"
#include "AI/AIGuardController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "Basic/GuardCharacter.h"
#include "Tools/Debug/DebugTools.h"

EBTNodeResult::Type UShootBTTaskNodeUtility::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("Shoot"));
	Debugger::get().addTextLog("Shoot","ia");
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	Cast<ATyranCharacter>(AIGuardController->GetPawn())->OnStartFire();
	Cast<ATyranCharacter>(AIGuardController->GetPawn())->OnStopFire();
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}

void UShootBTTaskNodeUtility::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

FString UShootBTTaskNodeUtility::GetStaticDescription() const
{
	return TEXT("Vise et tire sur un joueur");
}

void UShootBTTaskNodeUtility::CalculUtility(UBehaviorTreeComponent & OwnerComp)
{
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));

	if (HeroCharacterActor)
		utility = 600 / (FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation()));
	else
		utility = 0.0f;
	//distance voulue/ distance a la cible actuelle

	clampUtility();
}

