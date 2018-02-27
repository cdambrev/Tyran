// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CityGenerator.h"
#include "proceduralRoad.generated.h"

UCLASS()
class TYRAN_API AproceduralRoad : public AActor
{
	GENERATED_BODY()
	
public:	
	float halfSizeBigRoad;
	float halfSizeRoad;
	float halfSizePath;

	float sideSizeBigRoad;
	float sideSizeRoad;
	float sideSizePath;

	float sideHeightBigRoad;
	float sideHeightRoad;
	float sideHeightPath;

	// Sets default values for this actor's properties
	AproceduralRoad();
	UCustomMeshComponent * roadMesh;

	void buildBigRoad(float startX, float startY, float endX, float endY, float radBegin, float radEnd);
	void buildRoad(float startX, float startY, float endX, float endY, float radBegin, float radEnd);
	void buildPath(float startX, float startY, float endX, float endY, float radBegin, float radEnd);
	void buildCross(ACityGenerator::Crossroad * c, float radius);

protected:
	void buildParametrizedRoad(float startX, float startY, float endX, float endY, float halfSize, float sideWalkSize, float sizeWalkHeight, float radBegin, float radEnd);
};
