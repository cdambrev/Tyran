// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/MaterialBillboardComponent.h"
//#include "PaperSpriteComponent.h"
//#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//sprite2D = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Billboard"));
	BillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Billboard"));
	BillboardComponent->AttachTo(this);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextRender->AttachTo(BillboardComponent);
	TextRender->SetText(FText::FromString("Interact"));
	//TextRender->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	//TextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;

	BillboardComponent->SetHiddenInGame(true);
	TextRender->SetHiddenInGame(true);

}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInteractionComponent::OnBeginFocus()
{
	BillboardComponent->SetHiddenInGame(false);
	TextRender->SetHiddenInGame(false);
}

void UInteractionComponent::OnEndFocus()
{
	BillboardComponent->SetHiddenInGame(true);
	TextRender->SetHiddenInGame(true);
}
// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator billRotate = BillboardComponent->RelativeRotation;
	TextRender->SetRelativeRotation(billRotate);
}

