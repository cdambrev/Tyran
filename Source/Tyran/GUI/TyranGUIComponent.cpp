// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranGUIComponent.h"
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <ConstructorHelpers.h>


// Sets default values for this component's properties
UTyranGUIComponent::UTyranGUIComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> managerUIHelper(TEXT("/Game/UI/ManagerInterface"));
	managerUIClass = managerUIHelper.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> guardOrderUIHelper(TEXT("/Game/UI/GuardOrder"));
	guardUIClass = guardOrderUIHelper.Class;
}


// Called when the game starts
void UTyranGUIComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTyranGUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTyranGUIComponent::initAllGui_Implementation() {
	// création de tous les widgets
	UUserWidget * defaultGUI = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwner()->GetInstigatorController()), managerUIClass);
	defaultGUI->AddToViewport(9998);

	static_cast<APlayerController*>(GetOwner()->GetInstigatorController())->SetInputMode(FInputModeGameAndUI());
}

void UTyranGUIComponent::displayGuardUI_Implementation(FVector2D mouseLocation) {
	guardOrderWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwner()->GetInstigatorController()), guardUIClass);
	guardOrderWidget->AddToViewport(9999);
	guardOrderWidget->SetPositionInViewport(mouseLocation);
	guardOrderWidget->bIsFocusable = true;
}

void UTyranGUIComponent::removeGuardUI_Implementation() {
	guardOrderWidget->RemoveFromViewport();
}

void UTyranGUIComponent::displayDefaultGUI_Implementation() {
	defaultGUI->SetVisibility(ESlateVisibility::Visible);
}

void UTyranGUIComponent::removeDefaultGUI_Implementation() {
	defaultGUI->SetVisibility(ESlateVisibility::Hidden);
}

void UTyranGUIComponent::displayVictoryGUI_Implementation() {

}

void UTyranGUIComponent::removeVictoryGUI_Implementation() {

}

void UTyranGUIComponent::displayDefeatGUI_Implementation() {

}

void UTyranGUIComponent::removeDefeatGUI_Implementation() {

}

bool UTyranGUIComponent::isGuardUIDisplayed() {
	return (guardOrderWidget != nullptr);
}
