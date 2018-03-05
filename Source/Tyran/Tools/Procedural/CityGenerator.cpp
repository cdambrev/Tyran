// Fill out your copyright notice in the Description page of Project Settings.

#include "CityGenerator.h"
#include "EngineUtils.h"
#include "proceduralRoad.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ACityGenerator::ACityGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	nbIterationsBigRoadsGeneration = 2;
	nbIterationsRoadsGeneration = 6;
	nbIterationsPathsGeneration = 10;

	lastElementId = 0;
	BigRoadLengthMin = 80000;
	RoadLengthMin = 10000;
	PathLengthMin = 3100;
	BigRoadLengthMax = 120000;
	RoadLengthMax = 20000;
	PathLengthMax = 10000;

	BigCrossRoadBigRoadAmountMin = 1;
	BigCrossRoadBigRoadAmountMax = 3;
	BigCrossRoadRoadAmountMin = 1;
	BigCrossRoadRoadAmountMax = 4;
	BigCrossRoadPathAmountMin = 0;
	BigCrossRoadPathAmountMax = 2;

	SquaredMergeBigCrossroadDistance = 6000 * 6000;
	SquaredMergeCrossroadDistance = 6000 * 6000;
	SquaredMergeCrossPathDistance = 3000 * 3000;
	minAngleBetweenRoads = PI / 5;
	numberOfTryOnAngleFail = 50;

	BigCrossroadRadius = 1000.0f;
	CrossroadRadius = 800.0f;
	CrossPathRadius = 400.0f;

	halfSizeBigRoad = 500;
	halfSizeRoad = 300;
	halfSizePath = 200;
	sideHeightBigRoad = 10;
	sideHeightRoad = 10;
	sideHeightPath = 10;
	sideSizeBigRoad = 200;
	sideSizeRoad = 100;
	sideSizePath = 100;

	static ConstructorHelpers::FClassFinder<ABuildingSlot> buildSlotHelper(TEXT("/Game/Blueprints/BP_BuildingSlot"));
	buildSlot = buildSlotHelper.Class;
}

// Called when the game starts or when spawned
void ACityGenerator::BeginPlay()
{
	Super::BeginPlay();
	generateRoads();
	buildRoads();
	buildCrossroads();
}

void ACityGenerator::generateRoads() {
	crossroads.Add(new Crossroad{ 0.0f,0.0f,3 });
	for (int i = 0; i < nbIterationsBigRoadsGeneration; ++i) {
		TArray<ACityGenerator::Crossroad *> tempCrossroads{ crossroads };
		for (auto c : tempCrossroads) {
			if (c->isSeeding && c->level == 3) {
				seedFromCrossRoad(c, BigRoadLengthMin, BigRoadLengthMax, BigCrossRoadBigRoadAmountMin, BigCrossRoadBigRoadAmountMax, 3, SquaredMergeBigCrossroadDistance);
			}
		}
	}
	splitRoads(3, 2, SquaredMergeCrossroadDistance);
	for (int i = 0; i < nbIterationsRoadsGeneration; ++i) {
		TArray<ACityGenerator::Crossroad *> tempCrossroads{ crossroads };
		for (auto c : tempCrossroads) {
			if (c->isSeeding && c->level == 2) {
				seedFromCrossRoad(c, RoadLengthMin, RoadLengthMax, BigCrossRoadRoadAmountMin, BigCrossRoadRoadAmountMax, 2, SquaredMergeCrossroadDistance);
			}
		}
	}
	splitRoads(2, 1, SquaredMergeCrossPathDistance);
	for (int i = 0; i < nbIterationsPathsGeneration; ++i) {
		TArray<ACityGenerator::Crossroad *> tempCrossroads{ crossroads };
		for (auto c : tempCrossroads) {
			if (c->isSeeding && c->level == 1) {
				seedFromCrossRoad(c, PathLengthMin, PathLengthMax, BigCrossRoadPathAmountMin, BigCrossRoadPathAmountMax, 1, SquaredMergeCrossPathDistance);
			}
		}
	}
}

void ACityGenerator::seedFromCrossRoad(Crossroad * c, float lengthMin, float lengthMax, int amountMin, int amountMax, int roadLevel, float squaredMergeDistance) {
	int nbRoad = FMath::RandRange(amountMin, amountMax);
	for (int i = 0; i < nbRoad; ++i) {
		int count = 0;
		bool done = false;
		while (count < numberOfTryOnAngleFail && !done) {
			float Angles[] = {90, 180, 270};
			int choice = FMath::RandRange(0, 2);
			float angle = Angles[choice];
			float roadLength = FMath::RandRange(lengthMin, lengthMax);
			FVector2D dir(c->dirX, c->dirY);
			auto rDir = dir.GetRotated(angle);
			float crossX = c->posX + roadLength*rDir.X;
			float crossY = c->posY + roadLength*rDir.Y;
			Crossroad * mC = getOverlappingCrossRoad(crossX, crossY, squaredMergeDistance);
			if (mC) {
				if (checkExistingRoads(c, mC)) {
					Road * road = new Road(c, mC, roadLevel);
					roads.Add(road);
					c->connected.Add(road);
					mC->connected.Add(road);
					done = true;
				}
				else {
					++count;
				}
			}
			else {
				Crossroad * nC = new Crossroad(crossX, crossY, roadLevel, rDir.X, rDir.Y);
				if (checkExistingRoads(c, nC)) {
					crossroads.Add(nC);
					Road * road = new Road(c, nC, roadLevel);
					roads.Add(road);
					c->connected.Add(road);
					nC->connected.Add(road);
					done = true;
				}
				else {
					++count;
				}
			}
		}
	}
	c->isSeeding = false;
}

void ACityGenerator::splitRoads(int roadLevel, int crossroadLevel, float squaredMergeDistance)
{
	for (int i = 0; i < 20; ++i) {
		auto tempRoads = roads;
		for (Road * r : tempRoads) {
			if (r->level == roadLevel) {
				float roadSize = sqrt((r->endPoint->posX - r->beginPoint->posX) * (r->endPoint->posX - r->beginPoint->posX) + (r->endPoint->posY - r->beginPoint->posY) * (r->endPoint->posY - r->beginPoint->posY));
				float crossDist = FMath::RandRange(6000.0f, roadSize - 6000.0f);
				FVector2D dir{ r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY };
				dir.Normalize();
				if (!getOverlappingCrossRoad(r->beginPoint->posX + crossDist*dir.X, r->beginPoint->posY + crossDist*dir.Y, squaredMergeDistance)) {
					Crossroad * c = new Crossroad(r->beginPoint->posX + crossDist*dir.X, r->beginPoint->posY + crossDist*dir.Y, crossroadLevel, r->beginPoint->dirX, r->beginPoint->dirY);
					crossroads.Add(c);
					Road * nR = new Road(c, r->endPoint, roadLevel);
					roads.Add(nR);
					r->endPoint->connected.Remove(r);
					r->endPoint->connected.Add(nR);
					r->endPoint = c;
					c->connected.Add(r);
					c->connected.Add(nR);
				}
			}
		}
	}
}

void ACityGenerator::buildRoads() {
	for (auto r : roads) {
		auto road = GetWorld()->SpawnActor<AproceduralRoad>(AproceduralRoad::StaticClass(), GetActorTransform());
		road->initValues(halfSizeBigRoad, halfSizeRoad, halfSizePath, sideSizeBigRoad, sideSizeRoad, sideSizePath, sideHeightBigRoad, sideHeightRoad, sideHeightPath);
		float bRadius;
		float eRadius;
		if (r->beginPoint->level == 3) {
			bRadius = BigCrossroadRadius;
		}
		else if (r->beginPoint->level == 2) {
			bRadius = CrossroadRadius;
		}
		else {
			bRadius = CrossPathRadius;
		}
		if (r->endPoint->level == 3) {
			eRadius = BigCrossroadRadius;
		}
		else if (r->endPoint->level == 2) {
			eRadius = CrossroadRadius;
		}
		else {
			eRadius = CrossPathRadius;
		}
		if (r->level == 3) {
			road->buildBigRoad(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY, bRadius, eRadius);
		}
		else if (r->level == 2) {
			road->buildRoad(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY, bRadius, eRadius);
		}
		else if (r->level == 1) {
			road->buildPath(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY, bRadius, eRadius);
		}
	}
}

void ACityGenerator::buildCrossroads()
{
	for (auto c : crossroads) {
		auto cross = GetWorld()->SpawnActor<AproceduralRoad>(AproceduralRoad::StaticClass(), GetActorTransform());
		cross->initValues(halfSizeBigRoad, halfSizeRoad, halfSizePath, sideSizeBigRoad, sideSizeRoad, sideSizePath, sideHeightBigRoad, sideHeightRoad, sideHeightPath);
		float radius;
		if (c->level == 3) {
			radius = BigCrossroadRadius;
		}
		else if (c->level == 2) {
			radius = CrossroadRadius;
		}
		else {
			radius = CrossPathRadius;
		}
		cross->buildCross(c, radius);
	}
}

// Called every frame
void ACityGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACityGenerator::Crossroad * ACityGenerator::getOverlappingCrossRoad(float x, float y, float squaredDistance)
{
	for (auto c : crossroads) {
		if (((c->posX - x) * (c->posX - x) + (c->posY - y) * (c->posY - y)) < squaredDistance) {
			return c;
		}
	}
	return nullptr;
}

bool ACityGenerator::checkExistingRoads(ACityGenerator::Crossroad * c1, ACityGenerator::Crossroad * c2) {
	//Check for minimal angle
	FVector2D dir{ c2->posX - c1->posX, c2->posY - c1->posY };
	dir.Normalize();
	float cosMax = cos(minAngleBetweenRoads);
	for (Road * r : c1->connected) {
		FVector2D dirRef;
		if (r->beginPoint == c1) {
			dirRef = FVector2D( r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY );
		}
		else {
			dirRef = FVector2D(r->beginPoint->posX - r->endPoint->posX, r->beginPoint->posY - r->endPoint->posY);
		}
		dirRef.Normalize();
		float cosA = FVector2D::DotProduct(dir, dirRef);
		if (cosA > cosMax) {
			return false;
		}
	}
	for (Road * r : c2->connected) {
		FVector2D dirRef;
		if (r->beginPoint == c2) {
			dirRef = FVector2D(r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY);
		}
		else {
			dirRef = FVector2D(r->beginPoint->posX - r->endPoint->posX, r->beginPoint->posY - r->endPoint->posY);
		}
		dirRef.Normalize();
		float cosA = FVector2D::DotProduct(dir, dirRef);
		if (cosA < -cosMax) {
			return false;
		}
	}
	//Check for crossing roads
	for (Road * r : roads) {
		float Ix = c2->posX - c1->posX;
		float Iy = c2->posY - c1->posY;
		float Jx = r->endPoint->posX - r->beginPoint->posX;
		float Jy = r->endPoint->posY - r->beginPoint->posY;
		float m = 0, k = 0, div = Ix * Jy - Iy * Jx;
		if (div) {
			m = (Ix * c1->posY - Ix * r->beginPoint->posY - Iy * c1->posX + Iy *  r->beginPoint->posX) / div;
			k = (Jx * c1->posY - Jx * r->beginPoint->posY - Jy * c1->posX + Jy *  r->beginPoint->posX) / div;
			if ((m > 0 && m < 1) && (k > 0 && k < 1)) {
				return false;
				// position d'intersection : c1 + m * J);
			}
		}
	}
	return true;
}