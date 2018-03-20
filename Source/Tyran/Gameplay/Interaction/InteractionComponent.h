// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Basic/TyranCharacter.h"
#include "InteractionComponent.generated.h"


UCLASS(abstract, ClassGroup=(Custom))
class TYRAN_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//A UBillboardComponent to hold Icon sprite
	class UMaterialBillboardComponent* BillboardComponent;
	//Sprite for the Billboard Component
	class UTextRenderComponent * TextRender;
public:

	/* Le joueur regarde l'objet */
	virtual void OnBeginFocus();

	/* Le joueur arrête de regarder l'objet */
	virtual void OnEndFocus();

	/* Appelé quand le joueur interagit avec l'objet */
	virtual void OnUsed(ATyranCharacter* InstigatorPawn) { check(0 && "You must override this"); }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
