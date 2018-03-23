// Fill out your copyright notice in the Description page of Project Settings.

#include "LootInteractionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/item/Loot.h"




void ULootInteractionComponent::BeginPlay()
{
	loot = Cast<ALoot>(GetOwner());
}

void ULootInteractionComponent::OnBeginFocus()
{
	Super::OnBeginFocus();
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	loot->MeshComp->SetRenderCustomDepth(true);
}

void ULootInteractionComponent::OnEndFocus()
{
	Super::OnEndFocus();
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	loot->MeshComp->SetRenderCustomDepth(false);
}

void ULootInteractionComponent::OnUsed(ATyranCharacter* InstigatorPawn)
{
	loot->OnUsed(InstigatorPawn);
}
