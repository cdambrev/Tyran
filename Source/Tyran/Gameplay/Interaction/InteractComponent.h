// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Basic/TyranCharacter.h"
#include "InteractComponent.generated.h"


UCLASS(abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TYRAN_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

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
