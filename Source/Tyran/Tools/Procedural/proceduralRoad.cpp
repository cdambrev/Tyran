// Fill out your copyright notice in the Description page of Project Settings.

#include "proceduralRoad.h"


// Sets default values
AproceduralRoad::AproceduralRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	roadMesh = CreateDefaultSubobject<UCustomMeshComponent>(TEXT("RoadMesh"));

}

void AproceduralRoad::buildBigRoad(float startX, float startY, float endX, float endY)
{
	float roadHalfSize = 1000;
	TArray<FVector> points;
	FVector2D dir;
	dir.X = endX - startX;
	dir.Y = endY - startY;
	dir.Normalize();

	auto right = dir.GetRotated(90);
	points.Add(FVector{ startX - (roadHalfSize * right.X), startY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ startX + (roadHalfSize * right.X), startY + (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX - (roadHalfSize * right.X), endY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX + (roadHalfSize * right.X), endY + (roadHalfSize * right.Y), 0 });

	TArray<FCustomMeshTriangle> triangles;
	FCustomMeshTriangle t1;
	t1.Vertex0 = points[3];
	t1.Vertex1 = points[2];
	t1.Vertex2 = points[0];
	FCustomMeshTriangle t2;
	t2.Vertex0 = points[0];
	t2.Vertex1 = points[1];
	t2.Vertex2 = points[3];
	triangles.Add(t1);
	triangles.Add(t2);

	roadMesh->SetCustomMeshTriangles(triangles);
	RootComponent = roadMesh;
}

void AproceduralRoad::buildRoad(float startX, float startY, float endX, float endY)
{
	float roadHalfSize = 500;
	TArray<FVector> points;
	FVector2D dir;
	dir.X = endX - startX;
	dir.Y = endY - startY;
	dir.Normalize();

	auto right = dir.GetRotated(90);
	points.Add(FVector{ startX - (roadHalfSize * right.X), startY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ startX + (roadHalfSize * right.X), startY + (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX - (roadHalfSize * right.X), endY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX + (roadHalfSize * right.X), endY + (roadHalfSize * right.Y), 0 });

	TArray<FCustomMeshTriangle> triangles;
	FCustomMeshTriangle t1;
	t1.Vertex0 = points[3];
	t1.Vertex1 = points[2];
	t1.Vertex2 = points[0];
	FCustomMeshTriangle t2;
	t2.Vertex0 = points[0];
	t2.Vertex1 = points[1];
	t2.Vertex2 = points[3];
	triangles.Add(t1);
	triangles.Add(t2);

	roadMesh->SetCustomMeshTriangles(triangles);
	RootComponent = roadMesh;
}

void AproceduralRoad::buildPath(float startX, float startY, float endX, float endY)
{
	float roadHalfSize = 200;
	TArray<FVector> points;
	FVector2D dir;
	dir.X = endX - startX;
	dir.Y = endY - startY;
	dir.Normalize();

	auto right = dir.GetRotated(90);
	points.Add(FVector{ startX - (roadHalfSize * right.X), startY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ startX + (roadHalfSize * right.X), startY + (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX - (roadHalfSize * right.X), endY - (roadHalfSize * right.Y), 0 });
	points.Add(FVector{ endX + (roadHalfSize * right.X), endY + (roadHalfSize * right.Y), 0 });

	TArray<FCustomMeshTriangle> triangles;
	FCustomMeshTriangle t1;
	t1.Vertex0 = points[3];
	t1.Vertex1 = points[2];
	t1.Vertex2 = points[0];
	FCustomMeshTriangle t2;
	t2.Vertex0 = points[0];
	t2.Vertex1 = points[1];
	t2.Vertex2 = points[3];
	triangles.Add(t1);
	triangles.Add(t2);

	roadMesh->SetCustomMeshTriangles(triangles);
	RootComponent = roadMesh;
}

void AproceduralRoad::buildCross(float startX, float startY, float radius)
{
}

