// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranHUD.h"
#include <Color.h>
#include <ConstructorHelpers.h>


ATyranHUD::ATyranHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> guardOrderUIHelper(TEXT("/Game/UI/GuardOrder"));
	guardUIClass = guardOrderUIHelper.Class;
}

void ATyranHUD::DrawHUD() {
	DrawText("COUCOU", FColor::Yellow, 0, 0);
}

void ATyranHUD::displayGuardOrder(FVector2D position) {
	guardOrderWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), guardUIClass);
	guardOrderWidget->AddToViewport(9999);
	guardOrderWidget->SetPositionInViewport(position);
	guardOrderWidget->bIsFocusable = true;
	static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
}

void ATyranHUD::removeGuardOrder() {
	guardOrderWidget->RemoveFromViewport();
}
