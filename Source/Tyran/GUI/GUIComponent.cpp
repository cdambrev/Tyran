// Fill out your copyright notice in the Description page of Project Settings.

#include "GUIComponent.h"

// Sets default values for this component's properties
UGUIComponent::UGUIComponent() {

}

// Called when the game starts
void UGUIComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void initAllGui_Implementation() { check(0 && "You must override this"); }

void displayDefaultGUI_Implementation() { check(0 && "You must override this"); }

void removeDefaultGUI_Implementation() { check(0 && "You must override this"); }

void displayVictoryGUI_Implementation() { check(0 && "You must override this"); }

void removeVictoryGUI_Implementation() { check(0 && "You must override this"); }

void displayDefeatGUI_Implementation() { check(0 && "You must override this"); }

void removeDefeatGUI_Implementation() { check(0 && "You must override this"); }