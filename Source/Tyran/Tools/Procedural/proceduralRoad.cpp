// Fill out your copyright notice in the Description page of Project Settings.

#include "proceduralRoad.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
AproceduralRoad::AproceduralRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	roadMesh = CreateDefaultSubobject<UCustomMeshComponent>(TEXT("RoadMesh"));
	halfSizeBigRoad = 500;
	halfSizeRoad = 300;
	halfSizePath = 200;
	sideHeightBigRoad = 10;
	sideHeightRoad = 10;
	sideHeightPath = 10;
	sideSizeBigRoad = 200;
	sideSizeRoad = 100;
	sideSizePath = 100;
}

void AproceduralRoad::buildBigRoad(float startX, float startY, float endX, float endY, float radBegin, float radEnd)
{
	buildParametrizedRoad(startX, startY, endX, endY, halfSizeBigRoad, sideSizeBigRoad, sideHeightBigRoad, radBegin, radEnd);
}

void AproceduralRoad::buildRoad(float startX, float startY, float endX, float endY, float radBegin, float radEnd)
{
	buildParametrizedRoad(startX, startY, endX, endY, halfSizeRoad, sideSizeRoad, sideHeightRoad, radBegin, radEnd);
}

void AproceduralRoad::buildPath(float startX, float startY, float endX, float endY, float radBegin, float radEnd)
{
	buildParametrizedRoad(startX, startY, endX, endY, halfSizePath,  sideSizePath, sideHeightPath, radBegin, radEnd);
}

void AproceduralRoad::buildCross(ACityGenerator::Crossroad * c, float radius)
{
	TArray<TTuple<float, TArray<FVector>>> roads;
	bool first = true;
	FVector2D refDir;

	for (ACityGenerator::Road * r : c->connected) {
		float angle;
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

		float startX;
		float startY;
		float endX;
		float endY;

		if (r->beginPoint == c) {
			startX = r->beginPoint->posX;
			startY = r->beginPoint->posY;
			endX = r->endPoint->posX;
			endY = r->endPoint->posY;
		}
		else {
			endX = r->beginPoint->posX;
			endY = r->beginPoint->posY;
			startX = r->endPoint->posX;
			startY = r->endPoint->posY;
		}

		FVector2D dir;
		dir.X = endX - startX;
		dir.Y = endY - startY;
		dir.Normalize();

		if (first) {
			refDir = dir;
			angle = 0.0f;
			first = false;
		}
		else {
			float cosA = FVector2D::DotProduct(refDir, dir);
			float sinA = FVector2D::CrossProduct(refDir, dir);
			if (sinA > 0) {
				angle = -acos(cosA);
			}
			else {
				angle = acos(cosA);
			}
		}

		startX += dir.X*radius;
		startY += dir.Y*radius;

		auto right = dir.GetRotated(90);

		TArray<FVector> roadPoints;
		roadPoints.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), 0 });
		roadPoints.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), 0 });

		roadPoints.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), sideHeight});
		roadPoints.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), sideHeight});

		roadPoints.Add(FVector{ startX - ((halfSize + sideSize) * right.X), startY - ((halfSize + sideSize) * right.Y), sideHeight });
		roadPoints.Add(FVector{ startX + ((halfSize + sideSize) * right.X), startY + ((halfSize + sideSize) * right.Y), sideHeight });

		roads.Add(TTuple<float, TArray<FVector>>{angle,roadPoints});
	}
	roads.Sort([](TTuple<float, TArray<FVector>> left, TTuple<float, TArray<FVector>> right) {
		return left.Key > right.Key;
	});
	TArray<FCustomMeshTriangle> triangles;
	FVector center{ c->posX, c->posY, 0.0f };
	for (int i = 0; i < roads.Num(); ++i) {
		FCustomMeshTriangle inter;
		inter.Vertex0 = roads[i].Value[0];
		inter.Vertex1 = center;
		inter.Vertex2 = roads[i].Value[1];
		triangles.Add(inter);

		if (roads.Num() > 1) {
			FCustomMeshTriangle exter;
			FCustomMeshTriangle sideTop1;
			FCustomMeshTriangle sideTop2;
			if (i == roads.Num() - 1) {
				exter.Vertex2 = roads[0].Value[0];
				exter.Vertex1 = center;
				exter.Vertex0 = roads[i].Value[1];

				sideTop1.Vertex0 = roads[i].Value[3];
				sideTop1.Vertex1 = roads[0].Value[2];
				sideTop1.Vertex2 = roads[0].Value[4];

				sideTop2.Vertex0 = roads[i].Value[3];
				sideTop2.Vertex1 = roads[0].Value[4];
				sideTop2.Vertex2 = roads[i].Value[5];
			}
			else {
				exter.Vertex2 = roads[i + 1].Value[0];
				exter.Vertex1 = center;
				exter.Vertex0 = roads[i].Value[1];

				sideTop1.Vertex0 = roads[i].Value[3];
				sideTop1.Vertex1 = roads[i + 1].Value[2];
				sideTop1.Vertex2 = roads[i + 1].Value[4];

				sideTop2.Vertex0 = roads[i].Value[3];
				sideTop2.Vertex1 = roads[i + 1].Value[4];
				sideTop2.Vertex2 = roads[i].Value[5];
			}
			triangles.Add(exter);
			triangles.Add(sideTop1);
			triangles.Add(sideTop2);
		}
		
	}
	roadMesh->SetCustomMeshTriangles(triangles);
	RootComponent = roadMesh;
}

void AproceduralRoad::buildParametrizedRoad(float startX, float startY, float endX, float endY, float halfSize, float sideWalkSize, float sizeWalkHeight, float radBegin, float radEnd)
{
	TArray<FVector> points;
	FVector2D dir;
	dir.X = endX - startX;
	dir.Y = endY - startY;
	dir.Normalize();

	startX += dir.X*radBegin;
	startY += dir.Y*radBegin;

	endX -= dir.X*radEnd;
	endY -= dir.Y*radEnd;

	auto right = dir.GetRotated(90);
	points.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), 0 });
	points.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), 0 });
	points.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), sizeWalkHeight });
	points.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), sizeWalkHeight });
	points.Add(FVector{ startX - ((halfSize + sideWalkSize) * right.X), startY - ((halfSize + sideWalkSize)* right.Y), sizeWalkHeight });
	points.Add(FVector{ startX + ((halfSize + sideWalkSize) * right.X), startY + ((halfSize + sideWalkSize) * right.Y), sizeWalkHeight });

	points.Add(FVector{ endX - (halfSize * right.X), endY - (halfSize * right.Y), 0 });
	points.Add(FVector{ endX + (halfSize * right.X), endY + (halfSize * right.Y), 0 });
	points.Add(FVector{ endX - (halfSize * right.X), endY - (halfSize * right.Y), sizeWalkHeight });
	points.Add(FVector{ endX + (halfSize * right.X), endY + (halfSize * right.Y), sizeWalkHeight });
	points.Add(FVector{ endX - ((halfSize + sideWalkSize) * right.X), endY - ((halfSize + sideWalkSize)* right.Y), sizeWalkHeight });
	points.Add(FVector{ endX + ((halfSize + sideWalkSize) * right.X), endY + ((halfSize + sideWalkSize) * right.Y), sizeWalkHeight });

	TArray<FCustomMeshTriangle> triangles;

	FCustomMeshTriangle t1;//road
	t1.Vertex0 = points[7];
	t1.Vertex1 = points[6];
	t1.Vertex2 = points[0];
	FCustomMeshTriangle t2;
	t2.Vertex0 = points[0];
	t2.Vertex1 = points[1];
	t2.Vertex2 = points[7];

	FCustomMeshTriangle t3;//sidewalk interior left
	t3.Vertex0 = points[0];
	t3.Vertex1 = points[6];
	t3.Vertex2 = points[8];
	FCustomMeshTriangle t4;
	t4.Vertex0 = points[8];
	t4.Vertex1 = points[2];
	t4.Vertex2 = points[0];

	FCustomMeshTriangle t5;//sidewalk interior right
	t5.Vertex0 = points[3];
	t5.Vertex1 = points[9];
	t5.Vertex2 = points[7];
	FCustomMeshTriangle t6;
	t6.Vertex0 = points[7];
	t6.Vertex1 = points[1];
	t6.Vertex2 = points[3];

	FCustomMeshTriangle t7;//sidewalk top left
	t7.Vertex0 = points[10];
	t7.Vertex1 = points[4];
	t7.Vertex2 = points[2];
	FCustomMeshTriangle t8;
	t8.Vertex0 = points[2];
	t8.Vertex1 = points[8];
	t8.Vertex2 = points[10];

	FCustomMeshTriangle t9;//sidewalk top right
	t9.Vertex0 = points[11];
	t9.Vertex1 = points[9];
	t9.Vertex2 = points[3];
	FCustomMeshTriangle t10;
	t10.Vertex0 = points[3];
	t10.Vertex1 = points[5];
	t10.Vertex2 = points[11];


	triangles.Add(t1);
	triangles.Add(t2);
	triangles.Add(t3);
	triangles.Add(t4);
	triangles.Add(t5);
	triangles.Add(t6);
	triangles.Add(t7);
	triangles.Add(t8);
	triangles.Add(t9);
	triangles.Add(t10);

	roadMesh->SetCustomMeshTriangles(triangles);
	RootComponent = roadMesh;
}

