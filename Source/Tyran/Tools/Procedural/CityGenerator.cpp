// Fill out your copyright notice in the Description page of Project Settings.

#include "CityGenerator.h"
#include "EngineUtils.h"
#include "proceduralRoad.h"


// Sets default values
ACityGenerator::ACityGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	nbIterationsRoadsGeneration = 50;

	lastElementId = 0;
	BigRoadLengthMin = 10000;
	RoadLengthMin = 10000;
	PathLengthMin = 6000;
	BigRoadLengthMax = 50000;
	RoadLengthMax = 20000;
	PathLengthMax = 10000;

	BigCrossRoadBigRoadAmountMin = 2;
	BigCrossRoadBigRoadAmountMax = 4;
	BigCrossRoadRoadAmountMin = 1;
	BigCrossRoadRoadAmountMax = 2;
	BigCrossRoadPathAmountMin = 0;
	BigCrossRoadPathAmountMax = 2;

	SquaredMergeCrossroadDistance = 6000*6000;
	minAngleBetweenRoads = PI / 12;
	numberOfTryOnAngleFail = 50;
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
	for (int i = 0; i < nbIterationsRoadsGeneration; ++i) {
		auto tempCrossroads = crossroads;
		for (auto c : tempCrossroads) {
			if (c->isSeeding && c->level == 3) {
				seedFromCrossRoad(c, BigRoadLengthMin, BigRoadLengthMax, BigCrossRoadBigRoadAmountMin, BigCrossRoadBigRoadAmountMax, 3);
			}
		}
	}
	splitRoads(3, 2);
	for (int i = 0; i < nbIterationsRoadsGeneration; ++i) {
		auto tempCrossroads = crossroads;
		for (auto c : tempCrossroads) {
			if (c->isSeeding && c->level == 2) {
				seedFromCrossRoad(c, RoadLengthMin, RoadLengthMax, BigCrossRoadRoadAmountMin, BigCrossRoadRoadAmountMax, 2);
			}
		}
	}
}

void ACityGenerator::seedFromCrossRoad(Crossroad * c, float lengthMin, float lengthMax, int amountMin, int amountMax, int roadLevel) {
	int nbRoad = FMath::RandRange(amountMin, amountMax);
	for (int i = 0; i < nbRoad; ++i) {
		int count = 0;
		bool done = false;
		while (count < numberOfTryOnAngleFail && !done) {
			//LIMITED TO 45
			float Angles[] = { PI / 4,PI / 2, 3 * PI / 4, PI, -PI / 4, -PI / 2, -3 * PI / 4 };
			int choice = FMath::RandRange(0, 6);
			float angle = Angles[choice];
			//LIMITED to 90
			/*float Angles[] = {PI / 2, PI, -PI / 2};
			int choice = FMath::RandRange(0, 2);
			float angle = Angles[choice];*/
			//FREE
			/*float angle = FMath::RandRange(-PI, PI);*/
			//END ANGLE
			float roadLength = FMath::RandRange(lengthMin, lengthMax);
			float cosA = cos(angle);
			float sinA = sin(angle);
			float dirX = cosA*c->dirX - sinA*c->dirY;
			float dirY = sinA*c->dirX + cosA*c->dirY;
			float crossX = c->posX + roadLength*dirX;
			float crossY = c->posY + roadLength*dirY;
			Crossroad * mC = getOverlappingCrossRoad(crossX, crossY);
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
				Crossroad * nC = new Crossroad(crossX, crossY, roadLevel, dirX, dirY);
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

void ACityGenerator::splitRoads(int roadLevel, int crossroadLevel)
{
	for (int i = 0; i < 4; ++i) {
		auto tempRoads = roads;
		for (Road * r : tempRoads) {
			if (r->level == roadLevel && FMath::RandBool()) {
				float roadSize = sqrt((r->endPoint->posX - r->beginPoint->posX) * (r->endPoint->posX - r->beginPoint->posX) + (r->endPoint->posY - r->beginPoint->posY) * (r->endPoint->posY - r->beginPoint->posY));
				float crossDist = FMath::RandRange(0.0f, roadSize);
				FVector2D dir{ r->endPoint->posX - r->beginPoint->posX, r->beginPoint->posY };
				dir.Normalize();
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

void ACityGenerator::buildRoads() {
	for (auto r : roads) {
		auto road = GetWorld()->SpawnActor<AproceduralRoad>(AproceduralRoad::StaticClass(), GetActorTransform());
		if (r->level == 3) {
			road->buildBigRoad(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY);
		}
		else if (r->level == 2) {
			road->buildRoad(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY);
		}
		else if (r->level == 1) {
			road->buildPath(r->beginPoint->posX, r->beginPoint->posY, r->endPoint->posX, r->endPoint->posY);
		}
	}
}

void ACityGenerator::buildCrossroads()
{
}

// Called every frame
void ACityGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACityGenerator::Crossroad * ACityGenerator::getOverlappingCrossRoad(float x, float y)
{
	for (auto c : crossroads) {
		if (((c->posX - x) * (c->posX - x) + (c->posY - y) * (c->posY - y)) < SquaredMergeCrossroadDistance) {
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
		FVector2D dirRef{ r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY };
		dirRef.Normalize();
		float cosA = FVector2D::DotProduct(dir, dirRef);
		if (cosA > cosMax) {
			return false;
		}
	}
	for (Road * r : c2->connected) {
		FVector2D dirRef{ r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY };
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
			if ((m > 0.1 && m < 0.9) || (k > 0.1 && k < 0.9)) {
				return false;
				// position d'intersection : c1 + m * J);
			}
		}
	}
	return true;
}