// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

/**
 * 
 */
class TYRAN_API HeatMap
{
private:
	const uint16 maxSource = 32768;
	const uint16 maxPropagation = 16384;

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
	void setHeatAtPoint(FVector2D position, uint16 val);
	void init(UWorld * world);

	HeatMap();
	~HeatMap();
};
