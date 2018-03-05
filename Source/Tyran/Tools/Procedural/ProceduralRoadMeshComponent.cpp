// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralRoadMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"

UProceduralRoadMeshComponent::UProceduralRoadMeshComponent()
{
	ModelBodySetup = nullptr;
}

bool UProceduralRoadMeshComponent::GetPhysicsTriMeshData(FTriMeshCollisionData * CollisionData, bool InUseAllTriData)
{
	FTriIndices Triangle;

	for (int i = 0; i < localTriangles.Num(); ++i) {
		Triangle.v0 = CollisionData->Vertices.Add(localTriangles[i].Vertex0);
		Triangle.v1 = CollisionData->Vertices.Add(localTriangles[i].Vertex1);
		Triangle.v2 = CollisionData->Vertices.Add(localTriangles[i].Vertex2);

		CollisionData->Indices.Add(Triangle);
	}

	CollisionData->bFlipNormals = true;
	return true;
}

bool UProceduralRoadMeshComponent::ContainsPhysicsTriMeshData(bool InUseAllTriData) const
{
	return localTriangles.Num()>0;
}

void UProceduralRoadMeshComponent::setCustomTriangles(TArray<FCustomMeshTriangle> triangles)
{
	localTriangles = triangles;
	SetCustomMeshTriangles(localTriangles);
}

UBodySetup * UProceduralRoadMeshComponent::GetBodySetup()
{
	if (!ModelBodySetup) {
		ModelBodySetup = NewObject<UBodySetup>(UBodySetup::StaticClass());
		ModelBodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
		ModelBodySetup->bMeshCollideAll = true;
	}
	return ModelBodySetup;
}
