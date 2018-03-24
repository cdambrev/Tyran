// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranHUD.h"
#include <ConstructorHelpers.h>


ATyranHUD::ATyranHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> guardOrderUIHelper(TEXT("/Game/UI/GuardOrder"));
	guardUIClass = guardOrderUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> patrolPointsUIHelper(TEXT("/Game/UI/SetPatrouille"));
	patrolPointsModeUIClass = patrolPointsUIHelper.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> guardPointUIHelper(TEXT("/Game/UI/SetGuardPoint"));
	guardPointModeUIClass = guardPointUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> guardInfoUIHelper(TEXT("/Game/UI/GuardInfo"));
	guardInfoUIClass = guardInfoUIHelper.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> defaultUIHelper(TEXT("/Game/UI/ManagerInterface"));
	defaultUIClass = defaultUIHelper.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> endOfGameUIHelper(TEXT("/Game/UI/EndGame/TyranEndGameRevWin"));
	EndGameRevWinUIclass = endOfGameUIHelper.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> endOfGameUIHelper2(TEXT("/Game/UI/EndGame/TyranEndGameTyranWin"));
	EndGameTyranWinUIclass = endOfGameUIHelper2.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> timerUIHelper(TEXT("/Game/UI/Timer"));
	timerUIClass = timerUIHelper.Class;
}

void ATyranHUD::BeginPlay() {
	Super::BeginPlay();
	GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI());
	
	// default UI
	defaultUIWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), defaultUIClass);
	defaultUIWidget->AddToViewport(9998);
	
	// mode placer des points de patrouille
	patrolPointsMode = CreateWidget<UUserWidget>(GetOwningPlayerController(), patrolPointsModeUIClass);
	patrolPointsMode->SetVisibility(ESlateVisibility::Hidden);
	patrolPointsMode->AddToViewport(9999);
	patrolPointsMode->bIsFocusable = true;

	// mode placer des points de patrouille
	guardPointMode = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardPointModeUIClass);
	guardPointMode->SetVisibility(ESlateVisibility::Hidden);
	guardPointMode->AddToViewport(9999);
	guardPointMode->bIsFocusable = true;

	// mode affichage des ordres possibles
	guardOrderWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), guardUIClass);
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

void ATyranHUD::displayGuardPointMode() {
	guardPointMode->SetVisibility(ESlateVisibility::Visible);
}

void ATyranHUD::removeGuardPointMode() {
	guardPointMode->SetVisibility(ESlateVisibility::Hidden);
}

void ATyranHUD::displayGuardInfo() {
	guardInfo = CreateWidget<UUserWidget>(GetOwningPlayerController(), guardInfoUIClass);
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


void ATyranHUD::EndGameTyranWin() {
	removeAllPermanently();
	endOfGameUIWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), EndGameTyranWinUIclass);
	endOfGameUIWidget->AddToViewport(9998);

}

void ATyranHUD::EndGameRevWin() {
	removeAllPermanently();
	endOfGameUIWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), EndGameRevWinUIclass);
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


