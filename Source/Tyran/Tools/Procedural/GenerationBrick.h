// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "GameFramework/Actor.h"

/**
 * 
 */
class TYRAN_API GenerationBrick
{
public:
	struct Transformation{
		GenerationBrick * northWest;
		GenerationBrick * north;
		GenerationBrick * northEast;
		GenerationBrick * east;
		GenerationBrick * southEast;
		GenerationBrick * south;
		GenerationBrick * southWest;
		GenerationBrick * west;
		GenerationBrick * center;
		Transformation() {
			northWest = nullptr;
			north = nullptr;
			northEast = nullptr;
			east = nullptr;
			southEast = nullptr;
			south = nullptr;
			southWest = nullptr;
			west = nullptr;
			center = nullptr;
		}
	};

	int posX;
	int posY;
	Transformation finalTransformation;
	TSubclassOf<AActor> objectToBuild;
	bool invalidatedAtMerge;

private:
	TArray<Transformation> availableTransformations;
	TArray<int> transformationScore;
	int scoreSum;

public:
	void addTransformation(Transformation transform, int score) {
		availableTransformations.Add(transform);
		transformationScore.Add(score);
		scoreSum += score;
	}
	bool isTransformable() {
		return availableTransformations.Num() != 0;
	}
	Transformation getTransformation() {
		int score = FMath::RandRange(0, scoreSum);
		int tempSum = 0;
		for (int i = 0; i < availableTransformations.Num(); ++i) {
			tempSum += transformationScore[i];
			if (score <= tempSum) {
				return availableTransformations[i];
			}
		}
		return availableTransformations[0];
	}
	GenerationBrick() {
		scoreSum = 0;
		posX = 0;
		posY = 0;
		invalidatedAtMerge = false;
	}
	~GenerationBrick() {

	}
};