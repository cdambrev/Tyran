// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Basic/TyranCharacter.h"
#include "UsableObject.generated.h"

/**
 * 
 */
class ATyranCharacter;

USTRUCT(BlueprintType)
struct TYRAN_API FUsableObject //public UObject
{
	GENERATED_USTRUCT_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated)
	int stackPossible;

	int currentPossess;
	
	// marche pas avec struct
	//UPROPERTY(NotReplicated)
	//TSubclassOf<FUsableObject> objectClass;

	//UPROPERTY()
	//ATyranCharacter* myPawn;

public:
	FUsableObject();

	// ne pas oublier de retirer un objet
	virtual void onUse() { check(0 && "You must override this"); };
	//virtual TSubclassOf<FUsableObject> getObjectClass() { check(0 && "You must override this"); };

	void setOwner(ATyranCharacter* currCharacter) {
		//myPawn = currCharacter;
	}

	bool add() {
		if (currentPossess < stackPossible) {
			currentPossess++;
			return true;
		}
		return false;
	}
};
