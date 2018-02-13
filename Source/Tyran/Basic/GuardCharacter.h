// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TyranCharacter.h"
#include "GuardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AGuardCharacter : public ATyranCharacter
{
	GENERATED_BODY()

		
	
public:
	AGuardCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		int agression;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		bool flee;
	
};
