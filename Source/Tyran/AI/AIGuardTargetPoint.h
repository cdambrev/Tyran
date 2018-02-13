// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AIGuardTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAIGuardTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AAIGuardTargetPoint();

	/** Représente l'ordre dans lequel le NPC parcoure le chemin de patrouille ( 0 est le point initial ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables custom")
		int32 Position;

	
};
