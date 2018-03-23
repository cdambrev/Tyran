// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basic/TyranCharacter.h"
#include "UsableObject.generated.h"

/**
 * 
 */
class ATyranCharacter;

UCLASS(Blueprintable, BlueprintType)
class TYRAN_API UUsableObject : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int stackPossible;

	int currentPossess;
	
	TSubclassOf<UUsableObject> objectClass;

	UPROPERTY()
	ATyranCharacter* myPawn;

public:
	UUsableObject();

	// ne pas oublier de retirer un objet
	virtual void onUse() { check(0 && "You must override this"); };
	virtual bool add() { check(0 && "You must override this"); return false; };

	TSubclassOf<UUsableObject> getObjectClass();

	void setOwner(ATyranCharacter* currCharacter);

};
