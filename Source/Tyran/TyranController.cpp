// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranController.h"
#include <EngineUtils.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "AIGuardTargetPoint.h"

void ATyranController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranController, isTyran);
}

ATyranController::ATyranController() {
	isTyran = false;

}

//void ATyranController::onLeftClick() {
//	FHitResult hit;
//	GetHitResultUnderCursor(ECC_WorldStatic, false, hit);
//
//	if (hit.bBlockingHit) {
//		//UBlackboardComponent* blackboardComponent = BrainComponent->GetBlackboardComponent();
//		//blackboardComponent->
//		//
//		//BlackboardComponent->SetValueAsInt("TargetPointNumber", 0);
//		//hit.ImpactPoint;
//	}
//}