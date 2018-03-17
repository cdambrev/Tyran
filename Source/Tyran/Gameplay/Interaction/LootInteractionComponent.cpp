// Fill out your copyright notice in the Description page of Project Settings.

#include "LootInteractionComponent.h"
#include "Gameplay/item/Loot.h"




void ULootInteractionComponent::OnBeginFocus()
{
	ALoot* loot = Cast<ALoot>(GetOwner());
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	loot->OnBeginFocus();
}

void ULootInteractionComponent::OnEndFocus()
{
	ALoot* loot = Cast<ALoot>(GetOwner());
	// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
	loot->OnEndFocus();
}

void ULootInteractionComponent::OnUsed(ATyranCharacter* InstigatorPawn)
{
	ALoot* loot = Cast<ALoot>(GetOwner());
	loot->OnUsed(InstigatorPawn);
}
