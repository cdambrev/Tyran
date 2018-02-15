// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TyranCharacter.h"
#include <UnrealString.h>
#include "GuardCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ModeGuard : uint8 {
	OFFENSIF	UMETA(DisplayName="Offensif"),
	DEFENSIF	UMETA(DisplayName="Defensif"),
	TENIRPOSITION	UMETA(DisplayName="TenirPosition")
};

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
		ModeGuard modeGuard = ModeGuard::TENIRPOSITION;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool fuiteAutorise = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caractéristics")
		FString nom;
};
