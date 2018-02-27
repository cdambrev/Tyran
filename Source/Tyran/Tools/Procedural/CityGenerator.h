// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CityGenerator.generated.h"

UCLASS()
class TYRAN_API ACityGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	struct Road;
	struct Crossroad {
		float posX;
		float posY;
		float dirX;
		float dirY;
		int level;
		bool isSeeding;
		TArray<Road *> connected;
		Crossroad() {
			isSeeding = true;
		}
		Crossroad(float x, float y, int l, float dX = 1, float dY = 0) {
			posX = x;
			posY = y;
			level = l;
			dirX = dX;
			dirY = dY;
			isSeeding = true;
		}
	};

	struct Road {
		Crossroad * beginPoint;
		Crossroad * endPoint;
		int level;
		Road(Crossroad * b, Crossroad * e, int l) {
			beginPoint = b;
			endPoint = e;
			level = l;
		}
	};

	ACityGenerator();

	int lastElementId;

	TArray<Crossroad *> crossroads;
	TArray<Road *> roads;

	UPROPERTY(EditAnywhere)
	int nbIterationsBigRoadsGeneration;
	UPROPERTY(EditAnywhere)
	int nbIterationsRoadsGeneration;
	UPROPERTY(EditAnywhere)
	int nbIterationsPathsGeneration;

	UPROPERTY(EditAnywhere)
	float BigRoadLengthMin;
	UPROPERTY(EditAnywhere)
	float RoadLengthMin;
	UPROPERTY(EditAnywhere)
	float PathLengthMin;
	UPROPERTY(EditAnywhere)
	float BigRoadLengthMax;
	UPROPERTY(EditAnywhere)
	float RoadLengthMax;
	UPROPERTY(EditAnywhere)
	float PathLengthMax;

	UPROPERTY(EditAnywhere)
	int BigCrossRoadBigRoadAmountMin;
	UPROPERTY(EditAnywhere)
	int BigCrossRoadBigRoadAmountMax;
	UPROPERTY(EditAnywhere)
	int BigCrossRoadRoadAmountMin;
	UPROPERTY(EditAnywhere)
	int BigCrossRoadRoadAmountMax;
	UPROPERTY(EditAnywhere)
	int BigCrossRoadPathAmountMin;
	UPROPERTY(EditAnywhere)
	int BigCrossRoadPathAmountMax;

	UPROPERTY(EditAnywhere)
	float SquaredMergeBigCrossroadDistance;
	UPROPERTY(EditAnywhere)
	float SquaredMergeCrossroadDistance;
	UPROPERTY(EditAnywhere)
	float SquaredMergeCrossPathDistance;
	UPROPERTY(EditAnywhere)
	float minAngleBetweenRoads;
	UPROPERTY(EditAnywhere)
	int numberOfTryOnAngleFail;

	UPROPERTY(EditAnywhere)
	int BigCrossroadRadius;
	UPROPERTY(EditAnywhere)
	int CrossroadRadius;
	UPROPERTY(EditAnywhere)
	int CrossPathRadius;

protected:
	virtual void BeginPlay() override;
	void generateRoads();
	void buildRoads();
	void buildCrossroads();
	Crossroad * getOverlappingCrossRoad(float x, float y, float squaredDistance);
	bool checkExistingRoads(Crossroad * c1, Crossroad * c2);
	void seedFromCrossRoad(Crossroad * c, float lengthMin, float lengthMax, int amountMin, int amountMax, int roadLevel, float squaredMergeDistance);
	void splitRoads(int roadLevel, int crossroadLevel, float squaredMergeDistance);

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
