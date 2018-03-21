// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/MaterialBillboardComponent.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->AttachTo(this);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRender->AttachTo(BillboardComponent);
	TextRender->SetText(FText::FromString("Pickup Text"));
	//TextRender->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	//TextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;

	BillboardComponent->SetVisibility(true);
	TextRender->SetVisibility(true);

}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInteractionComponent::OnBeginFocus()
{
	BillboardComponent->SetVisibility(true);
	TextRender->SetVisibility(true);
}

void UInteractionComponent::OnEndFocus()
{
	BillboardComponent->SetVisibility(false);
	TextRender->SetVisibility(false);
}
// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

//	FRotator billRotate = BillboardComponent->
}

