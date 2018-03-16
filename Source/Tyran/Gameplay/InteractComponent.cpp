// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractComponent.h"


// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UInteractComponent::initFunctionPointer(void(*FunctionOnUsed))
{
	//OnUsed = Cast<void(APawn*)>(FunctionOnUsed);
}

void UInteractComponent::initFunctionPointer(void(*FunctionOnUsed), void(*FunctionOnBeginFocus), void(*FunctionOnEndFocus))
{

}

void UInteractComponent::setFunctionOnUsed(void(*Function))
{
}

void UInteractComponent::setFunctionOnBeginFocus(void(*Function))
{
	//OnBeginFocus = static_cast<(*void)>(Function);
}

void UInteractComponent::setFunctionOnEndFocus(void(*Function))
{

}

void UInteractComponent::OnBeginFocus_Default()
{

}

void UInteractComponent::OnEndFocus_Default(void)
{

}

void UInteractComponent::OnUsed_Default(APawn * InstigatorPawn)
{

}



// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!OnBeginFocus) {
		//OnBeginFocus = OnBeginFocus_Default;
	}
	if (!OnEndFocus) {
		//OnEndFocus = OnEndFocus_Default;
	}
	if (!OnUsed) {
		//OnUsed = Cast<void(APawn*)>(OnUsed_Default);
	}
	
}




// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

