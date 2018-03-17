// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <functional>
#include "Basic/TyranCharacter.h"
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

private:
	//ATyranCharacter* character;
	//typedef void(*voidFunctionPointerType)(void); // type for pointer function without parameters
	//typedef void(*APawnFunctionPointerType)(ATyranCharacter*); // type for pointer function without parameters
	//
	///* Le joueur regarde l'objet */
	//void(*OnBeginFocus_Default)();
	//void (*OnBeginFocus)();

	///* Le joueur arrête de regarder l'objet */
	//void(*OnEndFocus_Default)();
	//void (*OnEndFocus)();

	///* Appelé quand le joueur interagit avec l'objet */
	//void(*OnUsed_Default)(ATyranCharacter* InstigatorPawn);
	//void (*OnUsed)(ATyranCharacter* InstigatorPawn);

public:
	/*Redefinition des pointeur de fonction*/
	/*void initFunctionPointer(void(*function)(ATyranCharacter*));
	void initFunctionPointer(void(*function)(ATyranCharacter*), void(*functionOnBeginFocus)(), void(*functionOnEndFocus)());
	void setFunctionOnUsed(void(*function)(ATyranCharacter*));
	void setFunctionOnBeginFocus(void(*functionOnBeginFocus)());
	void setFunctionOnEndFocus(void(*functionOnEndFocus)());*/

	//void setCharacter(ATyranCharacter*);

	void interactFunction(ATyranCharacter* character) {
		character->DropWeapon();
	}

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
