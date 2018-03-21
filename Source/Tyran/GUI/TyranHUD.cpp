// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranHUD.h"
#include <ConstructorHelpers.h>


ATyranHUD::ATyranHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> guardOrderUIHelper(TEXT("/Game/UI/GuardOrder"));
	guardUIClass = guardOrderUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> patrolPointsUIHelper(TEXT("/Game/UI/SetPatrouille"));
	patrolPointsModeUIClass = patrolPointsUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> guardInfoUIHelper(TEXT("/Game/UI/GuardInfo"));
	guardInfoUIClass = guardInfoUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> defaultUIHelper(TEXT("/Game/UI/ManagerInterface"));
	defaultUIClass = defaultUIHelper.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> endOfGameUIHelper(TEXT("/Game/UI/EndOfGame"));
	endOfGameUIClass = endOfGameUIHelper.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> timerUIHelper(TEXT("/Game/UI/Timer"));
	timerUIClass = timerUIHelper.Class;
}

void ATyranHUD::BeginPlay() {
	Super::BeginPlay();
	static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
	
	// default UI
	defaultUIWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), defaultUIClass);
	defaultUIWidget->AddToViewport(9998);
	
	// mode placer des points de patrouille
	patrolPointsMode = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), patrolPointsModeUIClass);
	patrolPointsMode->SetVisibility(ESlateVisibility::Hidden);
	patrolPointsMode->AddToViewport(9999);
	patrolPointsMode->bIsFocusable = true;

	// mode affichage des ordres possibles
	guardOrderWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardUIClass);
	guardOrderWidget->SetVisibility(ESlateVisibility::Hidden);
	guardOrderWidget->AddToViewport(9999);
	guardOrderWidget->bIsFocusable = true;


	// default UI
	timerUIWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), timerUIClass);
	timerUIWidget->AddToViewport(9998);

	// affichage des informations sur le garde
	//guardInfo = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardInfoUIClass);
	//guardInfo->SetVisibility(ESlateVisibility::Hidden);
	//guardInfo->AddToViewport(9999);
	//guardInfo->bIsFocusable = false;

}
void ATyranHUD::displayGuardOrder(FVector2D position) {
	guardOrderWidget->SetPositionInViewport(position);
	guardOrderWidget->SetVisibility(ESlateVisibility::Visible);
	OnVisibleGuardOrder();
}

void ATyranHUD::removeGuardOrder() {
	guardOrderWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ATyranHUD::displayPatrolPointsMode() {
	patrolPointsMode->SetVisibility(ESlateVisibility::Visible);
}

void ATyranHUD::removePatrolPointsMode() {
	patrolPointsMode->SetVisibility(ESlateVisibility::Hidden);
}

void ATyranHUD::displayGuardInfo() {
	guardInfo = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardInfoUIClass);
	//guardInfo->SetVisibility(ESlateVisibility::Hidden);
	guardInfo->AddToViewport(9999);
	guardInfo->bIsFocusable = false;
	//guardInfo->SetVisibility(ESlateVisibility::Visible);
	//OnVisibleGuardInfo();
}

void ATyranHUD::removeGuardInfo() {
	guardInfo->RemoveFromViewport();
	//guardInfo->SetVisibility(ESlateVisibility::Hidden);
}

void ATyranHUD::displayDefaultUI() {
	defaultUIWidget->SetVisibility(ESlateVisibility::Visible);
}

void ATyranHUD::removeDefaultUI() {
	defaultUIWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ATyranHUD::modificationGuardInfoOffensif() {
	OnModificationGuardInfoOffensif();
}
void ATyranHUD::modificationGuardInfoDefensif() {
	OnModificationGuardInfoDefensif();
}
void ATyranHUD::modificationGuardInfoTenirPos() {
	OnModificationGuardInfoTenirPos();
}
void ATyranHUD::modificationGuardInfoFuite() {
	OnModificationGuardInfoFuite();
}


void ATyranHUD::OnEndOfGame() {
	removeAllPermanently();
	endOfGameUIWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), endOfGameUIClass);
	endOfGameUIWidget->AddToViewport(9998);

}

void ATyranHUD::removeAllPermanently() {
	if (defaultUIWidget)
	{
		defaultUIWidget->RemoveFromParent();
	}
	if (patrolPointsMode)
	{
		patrolPointsMode->RemoveFromParent();
	}
	if (guardOrderWidget)
	{
		guardOrderWidget->RemoveFromParent();
	}
	if (guardInfo)
	{
		guardInfo->RemoveFromParent();
	}
}