// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "HeatMap.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UHeatMap : public UObject
{
	GENERATED_BODY()
private:
	const float maxSource = 2.0f;
	const float maxPropagation = 1.0f;

	UTexture2D * map;
	int dimension;
	float pixelSize;
	FUpdateTextureRegion2D region;

public:
	UTexture2D * getTexture() {
		return map;
	}

	void update(float deltaTime);
	void addHeatSource(FVector2D pos);
	void setHeatAtPoint(FVector2D position, float val);
	void init(UWorld * world);

	UHeatMap();
	~UHeatMap();
	
	
	
};
