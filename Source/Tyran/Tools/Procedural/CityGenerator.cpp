// Fill out your copyright notice in the Description page of Project Settings.

#include "CityGenerator.h"
#include "EngineUtils.h"
#include "proceduralRoad.h"
#include "Gameplay/TyranOnly/Placeable_Object/BuildingSlot.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ACityGenerator::ACityGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	seed = 123456789;

	nbIterationsBigRoadsGeneration = 2;
	nbIterationsRoadsGeneration = 2;
	nbIterationsPathsGeneration = 3;

	lastElementId = 0;
	BigRoadLengthMin = 20000;
	RoadLengthMin = 12000;
	PathLengthMin = 4000;
	BigRoadLengthMax = 40000;
	RoadLengthMax = 30000;
	PathLengthMax = 10000;

	BigCrossRoadBigRoadAmountMin = 1;
	BigCrossRoadBigRoadAmountMax = 3;
	BigCrossRoadRoadAmountMin = 1;
	BigCrossRoadRoadAmountMax = 4;
	BigCrossRoadPathAmountMin = 0;
	BigCrossRoadPathAmountMax = 2;

	SquaredMergeBigCrossroadDistance = 12000 * 12000;
	SquaredMergeCrossroadDistance = 11900 * 11900;
	SquaredMergeCrossPathDistance = 3900 * 3900;
	minAngleBetweenRoads = PI / 5;
	numberOfTryOnAngleFail = 50;

	BigCrossroadRadius = 2000.0f;
	CrossroadRadius = 1600.0f;
	CrossPathRadius = 800.0f;

	halfSizeBigRoad = 700;
	halfSizeRoad = 500;
	halfSizePath = 300;
	sideHeightBigRoad = 10;
	sideHeightRoad = 10;
	sideHeightPath = 10;
	sideSizeBigRoad = 400;
	sideSizeRoad = 300;
	sideSizePath = 200;

	static ConstructorHelpers::FClassFinder<ABuildingSlot> buildSlotHelper(TEXT("/Game/Blueprints/BP_BuildingSlot"));
	buildSlot = buildSlotHelper.Class;
}

// Called when the game starts or when spawned
void ACityGenerator::BeginPlay()
{
	Super::BeginPlay();
	randStream = FRandomStream(seed);
	generateRoads();
	if (Role == ROLE_Authority) {
		placeBuildingSlots();
	}
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
	int nbRoad = randStream.RandRange(amountMin, amountMax);
	for (int i = 0; i < nbRoad; ++i) {
		int count = 0;
		bool done = false;
		while (count < numberOfTryOnAngleFail && !done) {
			float Angles[] = { 90, 180, 270 };
			int choice = randStream.RandRange(0, 3);
			float angle = Angles[choice];
			float roadLength = randStream.FRandRange(lengthMin, lengthMax);
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
				float crossDist = randStream.FRandRange(6000.0f, roadSize - 6000.0f);
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

bool ACityGenerator::pointIsLeftOf(FVector2D& p1, FVector2D& p2, FVector2D& p)
{
	/*float a = (-(p2.Y - p1.Y));
	float b = (p2.X - p1.X);
	return (a * p.X + b * p.Y + (-(a*p1.X + b*p1.Y)) >= 0);*/
	return FVector2D::CrossProduct(p2 - p1,p - p1) <= 0.0f;
}

bool ACityGenerator::pointIsRightOf(FVector2D& p1, FVector2D& p2, FVector2D& p)
{
	return FVector2D::CrossProduct(p2 - p1, p - p1) >= -0.0f;
}

bool ACityGenerator::checkSeparationLine(Rectangle& r1, Rectangle& r2, FVector2D& bEdge, FVector2D& eEdge)
{
	return (pointIsLeftOf(bEdge, eEdge, r1.p1) && pointIsLeftOf(bEdge, eEdge, r1.p2) && pointIsLeftOf(bEdge, eEdge, r1.p3) && pointIsLeftOf(bEdge, eEdge, r1.p4)
		&& pointIsRightOf(bEdge, eEdge, r2.p1) && pointIsRightOf(bEdge, eEdge, r2.p2) && pointIsRightOf(bEdge, eEdge, r2.p3) && pointIsRightOf(bEdge, eEdge, r2.p4)) ||
		(pointIsRightOf(bEdge, eEdge, r1.p1) && pointIsRightOf(bEdge, eEdge, r1.p2) && pointIsRightOf(bEdge, eEdge, r1.p3) && pointIsRightOf(bEdge, eEdge, r1.p4)
		&& pointIsLeftOf(bEdge, eEdge, r2.p1) && pointIsLeftOf(bEdge, eEdge, r2.p2) && pointIsLeftOf(bEdge, eEdge, r2.p3) && pointIsLeftOf(bEdge, eEdge, r2.p4));
}

/*bool ACityGenerator::pointIsInRectangle(FVector2D p, Rectangle rec)
{
	return pointIsLeftOf(rec.p2, rec.p1, p) && pointIsLeftOf(rec.p3, rec.p2, p) && pointIsLeftOf(rec.p4, rec.p3, p) && pointIsLeftOf(rec.p1, rec.p4, p);
}*/

bool ACityGenerator::rectanglesOverlap(Rectangle& r1, Rectangle& r2)
{
	/*	return pointIsInRectangle(r1.p1, r2) ||
			pointIsInRectangle(r1.p2, r2) ||
			pointIsInRectangle(r1.p3, r2) ||
			pointIsInRectangle(r1.p4, r2) ||
			pointIsInRectangle(r2.p1, r1) ||
			pointIsInRectangle(r2.p2, r1) ||
			pointIsInRectangle(r2.p3, r1) ||
			pointIsInRectangle(r2.p4, r1);*/
	return !(checkSeparationLine(r1, r2, r1.p1, r1.p2) ||
		checkSeparationLine(r1, r2, r1.p2, r1.p3) ||
		checkSeparationLine(r1, r2, r1.p3, r1.p4) ||
		checkSeparationLine(r1, r2, r1.p4, r1.p1) ||
		checkSeparationLine(r1, r2, r2.p1, r2.p2) ||
		checkSeparationLine(r1, r2, r2.p2, r2.p3) ||
		checkSeparationLine(r1, r2, r2.p3, r2.p4) ||
		checkSeparationLine(r1, r2, r2.p4, r2.p1));
}

void ACityGenerator::placeBuildingSlots()
{
	for (Road * r : roads) {
		float halfSize;
		float sideHeight;
		float sideSize;
		if (r->level == 3) {
			halfSize = halfSizeBigRoad;
			sideHeight = sideHeightBigRoad;
			sideSize = sideSizeBigRoad;
		}
		else if (r->level == 2) {
			halfSize = halfSizeRoad;
			sideHeight = sideHeightRoad;
			sideSize = sideSizeRoad;
		}
		else {
			halfSize = halfSizePath;
			sideHeight = sideHeightPath;
			sideSize = sideSizePath;
		}
		FVector dir{ r->endPoint->posX - r->beginPoint->posX, r->endPoint->posY - r->beginPoint->posY, 0 };
		float length = dir.Size();
		dir.Normalize();
		FVector right = dir.RotateAngleAxis(90, FVector{ 0.0f,0.0f,1.0f });
		FVector basePos{ r->beginPoint->posX, r->beginPoint->posY, GetActorTransform().GetLocation().Z + sideHeight };
		FRotator rot = right.Rotation();
		float relativePos = 1000;
		while (relativePos < length - 1000) {
			FVector pos = basePos + relativePos * dir + (halfSize + sideSize + 1000) * right;
			Rectangle nBR{ (pos + 1000 * dir - 1000 * right), (pos - 1000 * dir - 1000 * right), (pos - 1000 * dir + 1000 * right), (pos + 1000 * dir + 1000 * right) };
			FTransform trans{ rot,pos,FVector{1,1,1} };
			bool ok = true;
			for (TTuple<ABuildingSlot *,Rectangle> bs : slots) {
				if (rectanglesOverlap(bs.Value, nBR)) {
					ok = false;
				}
			}
			if (ok) {
				for (Road * r2 : roads) {
					float halfSize2;
					float sideHeight2;
					float sideSize2;
					if (r2->level == 3) {
						halfSize2 = halfSizeBigRoad-1;
						sideHeight2 = sideHeightBigRoad-1;
						sideSize2 = sideSizeBigRoad-1;
					}
					else if (r2->level == 2) {
						halfSize2 = halfSizeRoad-1;
						sideHeight2 = sideHeightRoad-1;
						sideSize2 = sideSizeRoad-1;
					}
					else {
						halfSize2 = halfSizePath-1;
						sideHeight2 = sideHeightPath-1;
						sideSize2 = sideSizePath-1;
					}
					FVector dir2{ r2->endPoint->posX - r2->beginPoint->posX, r2->endPoint->posY - r2->beginPoint->posY, 0 };
					dir2.Normalize();
					FVector right2 = dir2.RotateAngleAxis(90, FVector{ 0.0f,0.0f,1.0f });
					FVector begPoint{ r2->beginPoint->posX, r2->beginPoint->posY, 0.0f };
					FVector endPoint{ r2->endPoint->posX, r2->endPoint->posY, 0.0f };
					Rectangle roadRec{endPoint + right2*(halfSize2 + sideSize2),endPoint - right2*(halfSize2 + sideSize2), begPoint - right2*(halfSize2 + sideSize2), begPoint + right2 * (halfSize2 + sideSize2) };
					if (rectanglesOverlap(roadRec, nBR)) {
						ok = false;
					}
				}
			}
			if (ok) {
				auto build = GetWorld()->SpawnActor<ABuildingSlot>(buildSlot, trans);
				slots.Add(TTuple<ABuildingSlot *, Rectangle>{build, nBR});
				relativePos += 2000;
			}
			else {
				relativePos += 100;
			}
		}
		rot = (-right).Rotation();
		relativePos = 1000;
		while (relativePos < length - 1000) {
			FVector pos = basePos + relativePos * dir - (halfSize + sideSize + 1000) * right;
			Rectangle nBR{ (pos + 1000 * dir - 1000 * right), (pos - 1000 * dir - 1000 * right), (pos - 1000 * dir + 1000 * right), (pos + 1000 * dir + 1000 * right) };
			FTransform trans{ rot,pos,FVector{ 1,1,1 } };
			bool ok = true;
			for (TTuple<ABuildingSlot *, Rectangle> bs : slots) {
				if (rectanglesOverlap(bs.Value, nBR)) {
					ok = false;
				}
			}
			if (ok) {
				for (Road * r2 : roads) {
					float halfSize2;
					float sideHeight2;
					float sideSize2;
					if (r2->level == 3) {
						halfSize2 = halfSizeBigRoad - 1;
						sideHeight2 = sideHeightBigRoad - 1;
						sideSize2 = sideSizeBigRoad - 1;
					}
					else if (r2->level == 2) {
						halfSize2 = halfSizeRoad - 1;
						sideHeight2 = sideHeightRoad - 1;
						sideSize2 = sideSizeRoad - 1;
					}
					else {
						halfSize2 = halfSizePath - 1;
						sideHeight2 = sideHeightPath - 1;
						sideSize2 = sideSizePath - 1;
					}
					FVector dir2{ r2->endPoint->posX - r2->beginPoint->posX, r2->endPoint->posY - r2->beginPoint->posY, 0 };
					dir2.Normalize();
					FVector right2 = dir2.RotateAngleAxis(90, FVector{ 0.0f,0.0f,1.0f });
					FVector begPoint{ r2->beginPoint->posX, r2->beginPoint->posY, 0.0f };
					FVector endPoint{ r2->endPoint->posX, r2->endPoint->posY, 0.0f };
					Rectangle roadRec{ endPoint + right2*(halfSize2 + sideSize2),endPoint - right2*(halfSize2 + sideSize2), begPoint - right2*(halfSize2 + sideSize2), begPoint + right2 * (halfSize2 + sideSize2) };
					if (rectanglesOverlap(roadRec, nBR)) {
						ok = false;
					}
				}
			}
			if (ok) {
				auto build = GetWorld()->SpawnActor<ABuildingSlot>(buildSlot, trans);
				slots.Add(TTuple<ABuildingSlot *, Rectangle>{build, nBR});
				relativePos += 2000;
			}
			else {
				relativePos += 100;
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