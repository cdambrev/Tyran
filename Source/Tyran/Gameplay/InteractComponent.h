// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <functional>
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	void OnBeginFocus_Default(void);
	void (*OnBeginFocus)(void);

	/* Le joueur arrête de regarder l'objet */
	void OnEndFocus_Default(void);
	void (*OnEndFocus)(void);

	/* Appelé quand le joueur interagit avec l'objet */
	void OnUsed_Default(APawn* InstigatorPawn);
	void (*OnUsed)(APawn* InstigatorPawn);

	/*Redefinition des pointeur de fonction*/
	void initFunctionPointer(void(*FunctionOnUsed));
	void initFunctionPointer(void(*FunctionOnUsed), void(*FunctionOnBeginFocus), void(*FunctionOnEndFocus));
	void setFunctionOnUsed(void(*Function));
	void setFunctionOnBeginFocus(void(*Function));
	void setFunctionOnEndFocus(void(*Function));

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
