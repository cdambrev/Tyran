// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomMeshComponent.h"
#include "GameFramework/Actor.h"
#include "proceduralRoad.generated.h"

UCLASS()
class TYRAN_API AproceduralRoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AproceduralRoad();
	UCustomMeshComponent * roadMesh;

	void buildBigRoad(float startX, float startY, float endX, float endY);
	void buildRoad(float startX, float startY, float endX, float endY);
	void buildPath(float startX, float startY, float endX, float endY);
	void buildCross(float startX, float startY, float radius);
};
