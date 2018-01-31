// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "TyranCharacter.h"
#include "TyranViewSpotLightComponent.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UTyranViewSpotLightComponent : public USpotLightComponent
{
	GENERATED_BODY()
public:
	void BeginPlay() override;

	UTyranViewSpotLightComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float angleOfVision;
	float cosAoV;

	UFUNCTION(BlueprintCallable, Category = "Tyran")
	bool checkVisibility(AActor * actor);

	UFUNCTION(BlueprintCallable, Category = "Tyran")
	void tryToSee(ATyranCharacter * actor);
};
