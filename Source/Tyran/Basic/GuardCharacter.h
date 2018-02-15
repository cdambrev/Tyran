// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TyranCharacter.h"
#include <UnrealString.h>
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool offensif;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool deffensif;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool tenirPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool fuiteAutorise;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caractéristics")
		FString nom;
};
