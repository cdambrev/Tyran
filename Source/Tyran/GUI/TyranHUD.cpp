// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranHUD.h"
#include <Color.h>
#include <ConstructorHelpers.h>


ATyranHUD::ATyranHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> guardOrderUIHelper(TEXT("/Game/UI/GuardOrder"));
	guardUIClass = guardOrderUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> patrolPointsUIHelper(TEXT("/Game/UI/SetPatrouille"));
	patrolPointsModeUIClass = patrolPointsUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> guardInfoUIHelper(TEXT("/Game/UI/GuardInfo"));
	guardInfoUIClass = guardInfoUIHelper.Class;
}

void ATyranHUD::DrawHUD() {
}

void ATyranHUD::displayGuardOrder(FVector2D position) {
	guardOrderWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardUIClass);
	guardOrderWidget->AddToViewport(9999);
	guardOrderWidget->SetPositionInViewport(position);
	guardOrderWidget->bIsFocusable = true;
	static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
}

void ATyranHUD::removeGuardOrder() {
	if (guardOrderWidget != nullptr) {
		guardOrderWidget->RemoveFromViewport();
	}
}

void ATyranHUD::displayPatrolPointsMode() {
	patrolPointsMode = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), patrolPointsModeUIClass);
	patrolPointsMode->AddToViewport(9999);
	patrolPointsMode->bIsFocusable = true;
}

void ATyranHUD::removePatrolPointsMode() {
	patrolPointsMode->RemoveFromViewport();
}

void ATyranHUD::displayGuardInfo() {
	guardInfo = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardInfoUIClass);
	guardInfo->AddToViewport(9999);
	guardInfo->bIsFocusable = false;
}

void ATyranHUD::removeGuardInfo() {
	guardInfo->RemoveFromViewport();
}
