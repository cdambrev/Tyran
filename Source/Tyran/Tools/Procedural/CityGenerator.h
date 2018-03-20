// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/TyranOnly/Placeable_Object/BuildingSlot.h"
#include "Runtime/Core/Public/Math/RandomStream.h"
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

	struct Rectangle {
		FVector2D p1;
		FVector2D p2;
		FVector2D p3;
		FVector2D p4;
		Rectangle() = default;
		Rectangle(FVector _p1, FVector _p2, FVector _p3, FVector _p4) {
			p1.X = _p1.X;
			p1.Y = _p1.Y;
			p2.X = _p2.X;
			p2.Y = _p2.Y;
			p3.X = _p3.X;
			p3.Y = _p3.Y;
			p4.X = _p4.X;
			p4.Y = _p4.Y;
		}
	};

	ACityGenerator();

	int lastElementId;

	TArray<Crossroad *> crossroads;
	TArray<Road *> roads;
	TArray<TTuple<ABuildingSlot *, Rectangle>> slots;
	Rectangle bunkerRec;

	UPROPERTY(EditAnywhere)
	int seed;

	UPROPERTY(EditAnywhere)
	UMaterialInterface * roadMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface * sideMaterial;

	FRandomStream randStream;

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

	UPROPERTY(EditAnywhere)
	float halfSizeBigRoad;
	UPROPERTY(EditAnywhere)
	float halfSizeRoad;
	UPROPERTY(EditAnywhere)
	float halfSizePath;

	UPROPERTY(EditAnywhere)
	float sideSizeBigRoad;
	UPROPERTY(EditAnywhere)
	float sideSizeRoad;
	UPROPERTY(EditAnywhere)
	float sideSizePath;

	UPROPERTY(EditAnywhere)
	float sideHeightBigRoad;
	UPROPERTY(EditAnywhere)
	float sideHeightRoad;
	UPROPERTY(EditAnywhere)
	float sideHeightPath;

	TSubclassOf<ABuildingSlot> buildSlot;
	TSubclassOf<AActor> bunker;

protected:
	virtual void BeginPlay() override;
	void generateRoads();
	void buildRoads();
	void buildCrossroads();
	Crossroad * getOverlappingCrossRoad(float x, float y, float squaredDistance);
	bool checkExistingRoads(Crossroad * c1, Crossroad * c2, int level);
	void seedFromCrossRoad(Crossroad * c, float lengthMin, float lengthMax, int amountMin, int amountMax, int roadLevel, float squaredMergeDistance);
	void splitRoads(int roadLevel, int crossroadLevel, float squaredMergeDistance);
	bool pointIsLeftOf(FVector2D& p1, FVector2D& p2, FVector2D& p);
	bool pointIsRightOf(FVector2D& p1, FVector2D& p2, FVector2D& p);
	bool checkSeparationLine(Rectangle& r1, Rectangle& R2, FVector2D& begin, FVector2D& end);
	bool rectanglesOverlap(Rectangle& r1, Rectangle& r2);
	void placeBuildingSlots();

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};