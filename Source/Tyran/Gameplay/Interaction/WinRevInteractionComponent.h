// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Interaction/InteractionComponent.h"
#include "WinRevInteractionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TYRAN_API UWinRevInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:
	class UStaticMeshComponent* mesh; //temporaire juste pour highlight le cube 
	/* Le joueur regarde l'objet */
	virtual void OnBeginFocus() override;

	/* Le joueur arrête de regarder l'objet */
	virtual void OnEndFocus() override;

	/* Appelé quand le joueur interagit avec l'objet */
	virtual void OnUsed(ATyranCharacter* InstigatorPawn);
	
};
