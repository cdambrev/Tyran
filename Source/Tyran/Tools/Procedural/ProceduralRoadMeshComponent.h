// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomMeshComponent.h"
#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
#include "ProceduralRoadMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UProceduralRoadMeshComponent : public UCustomMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()
protected:
	TArray<FCustomMeshTriangle> localTriangles;

public:
	UProceduralRoadMeshComponent();

	class UBodySetup* ModelBodySetup;

	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;

	virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;

	virtual bool WantsNegXTriMesh() override { return false; };

	void setCustomTriangles(TArray<FCustomMeshTriangle> triangles);

	UBodySetup * GetBodySetup() override;
	
};
