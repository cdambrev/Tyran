// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TyranCharacter.h"
#include <UnrealString.h>
#include "Basic/Enum/TyranTypes.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int populationCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
		int agression;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		float porteeDeTir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		float vitesseDeDeplacement;

	UPROPERTY(Transient, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		ModeGuard modeGuard = ModeGuard::TENIRPOSITION;

	UPROPERTY(Transient, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Caracteristics")
		bool fuiteAutorise = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caracteristics")
		FString nom;

	
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


};
