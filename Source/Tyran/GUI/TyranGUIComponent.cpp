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
	UUserWidget * managerUI = CreateWidget<UUserWidget>(GetOwner()->GetGameInstance(), managerUIClass);
	managerUI->AddToViewport(9998);
}


// Called every frame
void UTyranGUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTyranGUIComponent::guardUI_Implementation(FVector2D mouseLocation) {
	if (guardOrderWidget) {
		guardOrderWidget->RemoveFromViewport();
	}
	guardOrderWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetController()), guardUIClass);
	guardOrderWidget->AddToViewport(9999);
	guardOrderWidget->SetPositionInViewport(mouseLocation);
	guardOrderWidget->bIsFocusable = true;
	static_cast<APlayerController*>(GetController())->SetInputMode(FInputModeGameAndUI());
}
