// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interaction/InteractionComponent.h"
#include "LootInteractionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TYRAN_API ULootInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:
	class ALoot* loot;
	/* Le joueur regarde l'objet */
	virtual void OnBeginFocus();

	/* Le joueur arrête de regarder l'objet */
	virtual void OnEndFocus();

	/* Appelé quand le joueur interagit avec l'objet */
	void OnUsed(ATyranCharacter* InstigatorPawn);
	
};
