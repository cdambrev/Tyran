// Fill out your copyright notice in the Description page of Project Settings.

#include "TyranGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


ATyranGameState::ATyranGameState(){

	static ConstructorHelpers::FClassFinder<UUserWidget> TimerObject(
		TEXT("/Game/Blueprints/Timer"));
	if (TimerObject.Class != NULL)
	{
		HUDWidgetClass = TimerObject.Class;
	}
}

FTimespan  ATyranGameState::GetTimeLeft() const {
	return FTimespan::FromSeconds(GameDuration-GetServerWorldTimeSeconds());
}


void ATyranGameState::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}
}