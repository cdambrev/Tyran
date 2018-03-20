// Fill out your copyright notice in the Description page of Project Settings.

#include "proceduralRoad.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
AproceduralRoad::AproceduralRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	roadMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RoadMesh"));
	RootComponent = roadMesh;
	roadMesh->bUseAsyncCooking = true;
}

void AproceduralRoad::initValues(float hSB, float hSR, float hSP, float sSB, float sSR, float sSP, float sHB, float sHR, float sHP, UMaterialInterface * rM, UMaterialInterface * sM)
{
	halfSizeBigRoad = hSB;
	halfSizeRoad = hSR;
	halfSizePath = hSP;
	sideHeightBigRoad = sHB;
	sideHeightRoad = sHR;
	sideHeightPath = sHP;
	sideSizeBigRoad = sSB;
	sideSizeRoad = sSR;
	sideSizePath = sSP;
	roadMat = rM;
	sizeMat = sM;
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
	TArray<TTuple<float, roadData>> roads;
	bool first = true;
	FVector2D refDir;

	for (ACityGenerator::Road * r : c->connected) {
		roadData rD;

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

		roadPoints.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), sideHeight });
		roadPoints.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), sideHeight });

		roadPoints.Add(FVector{ startX - ((halfSize + sideSize) * right.X), startY - ((halfSize + sideSize) * right.Y), sideHeight });
		roadPoints.Add(FVector{ startX + ((halfSize + sideSize) * right.X), startY + ((halfSize + sideSize) * right.Y), sideHeight });

		rD.dir = FVector{dir.X, dir.Y, 0.0f};
		rD.right = FVector{ right.X, right.Y, 0.0f };
		rD.halfSize = halfSize;
		rD.sideSize = sideSize;
		rD.points = roadPoints;

		roads.Add(TTuple<float, roadData>{angle, rD});
	}
	roads.Sort([](TTuple<float, roadData> left, TTuple<float, roadData> right) {
		return left.Key > right.Key;
	});
	TArray<FVector> points;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;
	TArray<int32> triangles;
	TArray<int32> triangles2;

	FVector center{ c->posX, c->posY, 0.0f };
	if (roads.Num() == 1) {

	}
	else {
		for (int i = 0; i < roads.Num(); ++i) {
			int next = (i + 1) % roads.Num();
			int prev = (i - 1);
			if (prev < 0) {
				prev += roads.Num();
			}
			//RoadLeft
			points.Add(roads[i].Value.points[0]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[0].X/1000,roads[i].Value.points[0].Y/1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//RoadRight
			points.Add(roads[i].Value.points[1]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[1].X/1000,roads[i].Value.points[1].Y/1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//RoadIntersection
			if (FVector::DotProduct(-(roads[i].Value.right), roads[prev].Value.dir) >= 0.01f || FVector::DotProduct(-(roads[i].Value.right), roads[prev].Value.dir) <= -0.01f) {
				points.Add(center + (roads[prev].Value.dir * roads[i].Value.halfSize /FVector::DotProduct(((-(roads[i].Value.right))), roads[prev].Value.dir))
					+ (roads[i].Value.dir * roads[prev].Value.halfSize /FVector::DotProduct(((roads[prev].Value.right)), roads[i].Value.dir)));
			}
			else{
				points.Add(center + roads[i].Value.halfSize * (-(roads[i].Value.right)));
			}
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ points.Last().X / 1000,points.Last().Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideLeftCenter
			points.Add(roads[i].Value.points[2]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[2].X / 1000,roads[i].Value.points[2].Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideRightCenter
			points.Add(roads[i].Value.points[3]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[3].X / 1000,roads[i].Value.points[3].Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideLeftExter
			points.Add(roads[i].Value.points[4]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[4].X / 1000,roads[i].Value.points[4].Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideRightExter
			points.Add(roads[i].Value.points[5]);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ roads[i].Value.points[5].X / 1000,roads[i].Value.points[5].Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideIntersection
			if (FVector::DotProduct((-(roads[i].Value.right)), roads[prev].Value.dir) >= 0.01f || FVector::DotProduct((-(roads[i].Value.right)), roads[prev].Value.dir) <= -0.01f) {
				points.Add(center + (roads[prev].Value.dir * (roads[i].Value.halfSize + roads[i].Value.sideSize) / FVector::DotProduct(((-(roads[i].Value.right))), roads[prev].Value.dir))
					+ (roads[i].Value.dir * (roads[prev].Value.halfSize + roads[prev].Value.sideSize) / FVector::DotProduct(((roads[prev].Value.right)), roads[i].Value.dir)));
			}
			else {
				points.Add(center + (roads[i].Value.halfSize + roads[i].Value.sideSize) * (-(roads[i].Value.right)));
			}
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ points.Last().X / 1000,points.Last().Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//SideIntersectionInter
			points.Add(points[points.Num() - 6] + FVector(0.0f,0.0f,10.0f));
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ points.Last().X / 1000,points.Last().Y / 1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));

			//Road
			triangles.Add((i * 9) + 1);
			triangles.Add((i * 9) + 0);
			triangles.Add((i * 9) + 2);

			triangles.Add((next * 9) + 2);
			triangles.Add((i * 9) + 1);
			triangles.Add((i * 9) + 2);

			//SideTop
			triangles2.Add(i * 9 + 6);
			triangles2.Add(i * 9 + 4);
			triangles2.Add(next * 9 + 7);

			triangles2.Add(i * 9 + 4);
			triangles2.Add(next * 9 + 8);
			triangles2.Add(next * 9 + 7);

			triangles2.Add(next * 9 + 8);
			triangles2.Add(next * 9 + 3);
			triangles2.Add(next * 9 + 7);

			triangles2.Add(next * 9 + 3);
			triangles2.Add(next * 9 + 5);
			triangles2.Add(next * 9 + 7);

			//SideSide
			triangles2.Add(next * 9 + 2);
			triangles2.Add(next * 9 + 8);
			triangles2.Add(i * 9 + 4);

			triangles2.Add(i * 9 + 4);
			triangles2.Add(i * 9 + 1);
			triangles2.Add(next * 9 + 2);

			triangles2.Add(next * 9 + 8);
			triangles2.Add(next * 9 + 2);
			triangles2.Add(next * 9 + 3);

			triangles2.Add(next * 9 + 8);
			triangles2.Add(next * 9 + 2);
			triangles2.Add(next * 9 + 0);

			triangles2.Add(next * 9 + 0);
			triangles2.Add(next * 9 + 3);
			triangles2.Add(next * 9 + 8);

			//CenterRoad
			if (roads.Num() > 2) {
				triangles.Add(roads.Num() * 9);
				triangles.Add((next * 9) + 2);
				triangles.Add((i * 9) + 2);
			}
		}
		if (roads.Num() > 2) {
			points.Add(center);
			normals.Add(FVector{ 0.0f,0.0f,1.0f });
			UV0.Add(FVector2D{ center.X/1000,center.Y/1000 });
			colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
			tangents.Add(FProcMeshTangent(0, 1, 0));
		}
	}	
	roadMesh->CreateMeshSection_LinearColor(0, points, triangles, normals, UV0, colors, tangents, true);
	roadMesh->CreateMeshSection_LinearColor(1, points, triangles2, normals, UV0, colors, tangents, true);
	roadMesh->SetMaterial(0, roadMat);
	roadMesh->SetMaterial(1, sizeMat);
	RootComponent = roadMesh;
}

void AproceduralRoad::buildParametrizedRoad(float startX, float startY, float endX, float endY, float halfSize, float sideWalkSize, float sizeWalkHeight, float radBegin, float radEnd)
{
	TArray<FVector> points;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> colors;
	TArray<FProcMeshTangent> tangents;

	FVector2D dir;
	dir.X = endX - startX;
	dir.Y = endY - startY;
	float size = dir.Size();
	dir.Normalize();

	startX += dir.X*radBegin;
	startY += dir.Y*radBegin;

	endX -= dir.X*radEnd;
	endY -= dir.Y*radEnd;

	auto right = dir.GetRotated(90);
	points.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), 0 });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX - (halfSize * right.X))/1000, (startY - (halfSize * right.Y))/1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), 0 });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX + (halfSize * right.X)) / 1000, (startY + (halfSize * right.Y))/1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ startX - (halfSize * right.X), startY - (halfSize * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX - (halfSize * right.X))/1000 , (startY - (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ startX + (halfSize * right.X), startY + (halfSize * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX + (halfSize * right.X))/1000, (startY + (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ startX - ((halfSize + sideWalkSize) * right.X), startY - ((halfSize + sideWalkSize)* right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX - ((halfSize + sideWalkSize) * right.X))/1000 ,(startY - ((halfSize + sideWalkSize)* right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ startX + ((halfSize + sideWalkSize) * right.X), startY + ((halfSize + sideWalkSize) * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (startX + ((halfSize + sideWalkSize) * right.X)) / 1000, (startY + ((halfSize + sideWalkSize) * right.Y))/1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	//----------------------------------

	points.Add(FVector{ endX - (halfSize * right.X), endY - (halfSize * right.Y), 0 });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX - (halfSize * right.X))/1000, (endY - (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ endX + (halfSize * right.X), endY + (halfSize * right.Y), 0 });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX + (halfSize * right.X)) / 1000, (endY + (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ endX - (halfSize * right.X), endY - (halfSize * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX - (halfSize * right.X)) / 1000, (endY - (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ endX + (halfSize * right.X), endY + (halfSize * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX + (halfSize * right.X)) / 1000, (endY + (halfSize * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ endX - ((halfSize + sideWalkSize) * right.X), endY - ((halfSize + sideWalkSize)* right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX - ((halfSize + sideWalkSize) * right.X)) / 1000, (endY - ((halfSize + sideWalkSize)* right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	points.Add(FVector{ endX + ((halfSize + sideWalkSize) * right.X), endY + ((halfSize + sideWalkSize) * right.Y), sizeWalkHeight });
	normals.Add(FVector{ 0,0,1 });
	UV0.Add(FVector2D{ (endX + ((halfSize + sideWalkSize) * right.X)) / 1000, (endY + ((halfSize + sideWalkSize) * right.Y)) / 1000 });
	colors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<int32> triangles;
	TArray<int32> triangles2;

	//road
	triangles.Add(7);
	triangles.Add(6);
	triangles.Add(0);
	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(7);

	//sidewalk interior left
	triangles2.Add(0);
	triangles2.Add(6);
	triangles2.Add(8);
	triangles2.Add(8);
	triangles2.Add(2);
	triangles2.Add(0);

	//sidewalk interior right
	triangles2.Add(3);
	triangles2.Add(9);
	triangles2.Add(7);
	triangles2.Add(7);
	triangles2.Add(1);
	triangles2.Add(3);

	//sidewalk top left
	triangles2.Add(10);
	triangles2.Add(4);
	triangles2.Add(2);
	triangles2.Add(2);
	triangles2.Add(8);
	triangles2.Add(10);

	//sidewalk top right
	triangles2.Add(11);
	triangles2.Add(9);
	triangles2.Add(3);
	triangles2.Add(3);
	triangles2.Add(5);
	triangles2.Add(11);

	roadMesh->CreateMeshSection_LinearColor(0,points,triangles,normals, UV0, colors, tangents, true);
	roadMesh->CreateMeshSection_LinearColor(1, points, triangles2, normals, UV0, colors, tangents, true);
	RootComponent = roadMesh;
	roadMesh->SetMaterial(0, roadMat);
	roadMesh->SetMaterial(1, sizeMat);
}

