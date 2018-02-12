// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranController.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "AIGuardTargetPoint.h"

void ATyranController::setTyran(bool b)
{
	isTyran = b;
	if (b) {
		initOnTyranClient();
	}
}

void ATyranController::initOnTyranClient_Implementation()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	UUserWidget * managerUI = CreateWidget<UUserWidget>(GetGameInstance(), managerUIClass);
	managerUI->AddToViewport(9999);
}

void ATyranController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranController, isTyran);
}

void ATyranController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
}

ATyranController::ATyranController() {
	isTyran = false;
	static ConstructorHelpers::FClassFinder<UUserWidget> managerUIHelper(TEXT("/Game/UI/ManagerInterface"));
	managerUIClass = managerUIHelper.Class;
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