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
	/**	 Interface for retrieving triangle mesh collision data from the implementing object
	*
	* @param CollisionData - structure given by the caller to be filled with tri mesh collision data
	* @return true if successful, false if unable to successfully fill in data structure
	*/
	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;

	/**	 Interface for checking if the implementing objects contains triangle mesh collision data
	*
	* @return true if the implementing object contains triangle mesh data, false otherwise
	*/
	virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;

	/** Do we want to create a negative version of this mesh */
	virtual bool WantsNegXTriMesh() override { return false; };

	void setCustomTriangles(TArray<FCustomMeshTriangle> triangles);

	UBodySetup * GetBodySetup() override;
	
};
