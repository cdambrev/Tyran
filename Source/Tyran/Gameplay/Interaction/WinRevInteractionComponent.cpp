// Fill out your copyright notice in the Description page of Project Settings.

#include "WinRevInteractionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Basic/TyranGameState.h"

void UWinRevInteractionComponent::BeginPlay()
{
	//GetActor()->FindComponentByClass<UStaticMeshComponent>()
	mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
}

void UWinRevInteractionComponent::OnBeginFocus()
{
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	mesh->SetRenderCustomDepth(true);
}

void UWinRevInteractionComponent::OnEndFocus()
{
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	mesh->SetRenderCustomDepth(false);
}

void UWinRevInteractionComponent::OnUsed(ATyranCharacter* InstigatorPawn)
{
	ATyranGameState* gamestate = Cast<ATyranGameState>(GetWorld()->GetGameState());
	gamestate->RevWin();
}
