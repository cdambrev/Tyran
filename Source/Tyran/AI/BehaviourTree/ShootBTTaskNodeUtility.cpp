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
#ifdef DEBUG_ON
	Debugger::get().addTextLog("Shoot","ia");
#endif

	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	if (HeroCharacterActor)
	{
		FVector TargetDirection = (HeroCharacterActor->GetActorLocation() - AIGuardController->GetPawn()->GetActorLocation()).GetSafeNormal();
		FVector forwardDirection = AIGuardController->GetPawn()->GetActorForwardVector();
		float produitMixte = FVector::DotProduct(FVector::CrossProduct(TargetDirection, forwardDirection), FVector(0.0f, 0.0f, 1.0f));
		bool isInFrontOf = FVector::DotProduct(TargetDirection, forwardDirection) > 0;
		FRotator guardRotator = AIGuardController->GetPawn()->GetActorRotation();
		if (produitMixte > 0.2 || produitMixte < -0.2)
		{
			guardRotator.Add(0, -20*produitMixte, 0);
			Cast<ATyranCharacter>(AIGuardController->GetPawn())->SetActorRotation(guardRotator);
		}
		else if (!isInFrontOf) {
			guardRotator.Add(0, -10, 0);
			Cast<ATyranCharacter>(AIGuardController->GetPawn())->SetActorRotation(guardRotator);
			//guardRotator.Add(0, -10*produitMixte, 0);
			//Cast<ATyranCharacter>(AIGuardController->GetPawn())->SetActorRotation(guardRotator);
		}
		else
		{
			if (Cast<ATyranCharacter>(AIGuardController->GetPawn())->getMagCurrent() > 0)
			{
				Cast<ATyranCharacter>(AIGuardController->GetPawn())->OnStartFire();
				Cast<ATyranCharacter>(AIGuardController->GetPawn())->OnStopFire();
			}
			else
			{
				Cast<ATyranCharacter>(AIGuardController->GetPawn())->OnReload();
			}
		}
	}

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

	if (HeroCharacterActor && checkVisibility(OwnerComp))
		utility = 600 / (FVector::Distance(HeroCharacterActor->GetActorLocation(), AIGuardController->GetPawn()->GetActorLocation()));
	else
		utility = 0.0f;
	//distance voulue/ distance a la cible actuelle

	clampUtility();
}

bool UShootBTTaskNodeUtility::checkVisibility(UBehaviorTreeComponent & OwnerComp) {
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());
	AActor* HeroCharacterActor = Cast<AActor>(AIGuardController->GetBlackboardComponent()->GetValueAsObject("TargetActorToFollow"));
	FVector dir = HeroCharacterActor->GetActorLocation() - AIGuardController->GetPawn()->GetActorLocation();
	dir.Normalize();
	// On ne vérifie pas l'angle de vision, normalement, car ca a déjà été vérifié en fixant si la cible est visible
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
