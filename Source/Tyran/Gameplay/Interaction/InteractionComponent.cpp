// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->SetupAttachment(RootComponent);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRender->AttachTo(RootComponent);
	TextRender->SetText(FText::FromString("Pickup Text"));
	TextRender->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	TextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;*/
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInteractionComponent::OnBeginFocus()
{

}

void UInteractionComponent::OnEndFocus()
{

}
// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

