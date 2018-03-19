// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interaction/InteractComponent.h"
#include "LootInteractionComponent.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TYRAN_API ULootInteractionComponent : public UInteractComponent
{
	GENERATED_BODY()


protected:	
	virtual void BeginPlay() override;
public:
	class ALoot* loot;
	/* Le joueur regarde l'objet */
	void OnBeginFocus() ;

	/* Le joueur arrête de regarder l'objet */
	void OnEndFocus() ;

	/* Appelé quand le joueur interagit avec l'objet */
	void OnUsed(ATyranCharacter* InstigatorPawn);


	
	
};
