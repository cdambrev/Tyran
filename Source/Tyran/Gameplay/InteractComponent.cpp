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
//void UInteractComponent::initFunctionPointer(void(*functionOnUsed)(ATyranCharacter*))
//{
//	OnUsed = functionOnUsed;
//}
//
//void UInteractComponent::initFunctionPointer(void(*functionOnUsed)(ATyranCharacter*), void(*functionOnBeginFocus)(), void(*functionOnEndFocus)())
//{
//	OnUsed = functionOnUsed;
//	OnBeginFocus = functionOnBeginFocus;
//	OnEndFocus = functionOnEndFocus;
//
//}
//
//void UInteractComponent::setFunctionOnUsed(void(*functionOnUsed)(ATyranCharacter*))
//{
//	OnUsed = functionOnUsed;
//}
//
//void UInteractComponent::setFunctionOnBeginFocus(void(*functionOnBeginFocus)())
//{
//	OnBeginFocus = functionOnBeginFocus;
//}
//
//void UInteractComponent::setFunctionOnEndFocus(void(*functionOnEndFocus)())
//{
//	OnEndFocus = functionOnEndFocus;
//}

//void UInteractComponent::setCharacter(ATyranCharacter* character) {
//	this->character = character;
//}


//
//void (UInteractComponent::*OnBeginFocus_Default)()
//{
//
//};
//
//void (UInteractComponent::*OnEndFocus_Default)()
//{
//
//};
//
//void (UInteractComponent::*OnUsed_Default)(ATyranCharacter * InstigatorPawn)
//{
//
//};



// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	//if (!OnBeginFocus) {
	//	OnBeginFocus = OnBeginFocus_Default;
	//}
	//if (!OnEndFocus) {
	//	OnEndFocus = OnEndFocus_Default;
	//}
	//if (!OnUsed) {
	//	OnUsed = OnUsed_Default;
	//}
	
}




// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

