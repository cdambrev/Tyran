// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Basic/GuardCharacter.h"
#include "GuardSpawnPoint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TYRAN_API UGuardSpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGuardSpawnPoint();

public:	
	void spawnGuard(TSubclassOf<AGuardCharacter> gClass);
};
