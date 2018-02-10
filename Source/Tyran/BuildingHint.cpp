// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingHint.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>


// Sets default values
ABuildingHint::ABuildingHint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> validMatFinder(TEXT("/Game/Objects/BuildingHints/ValidPositionMaterial"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> invalidMatFinder(TEXT("/Game/Objects/BuildingHints/InvalidPositionMaterial"));
	validMaterial = validMatFinder.Object;
	invalidMaterial = invalidMatFinder.Object;

}

// Called when the game starts or when spawned
void ABuildingHint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingHint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingHint::setValidPosition(bool b)
{
	if (b) {
		for (auto it = GetComponents().CreateConstIterator(); it; ++it)
		{
			UStaticMeshComponent * pMeshComponent = static_cast<UStaticMeshComponent *>(*it);

			if (pMeshComponent)
			{
				for (int i = 0; i < pMeshComponent->GetNumMaterials(); ++i)
				{
					pMeshComponent->SetMaterial(i, validMaterial);
				}
			}
		}
	}
	else {
		for (auto it = GetComponents().CreateConstIterator(); it; ++it)
		{
			UStaticMeshComponent * pMeshComponent = static_cast<UStaticMeshComponent *>(*it);

			if (pMeshComponent)
			{
				for (int i = 0; i < pMeshComponent->GetNumMaterials(); ++i)
				{
					pMeshComponent->SetMaterial(i, invalidMaterial);
				}
			}
		}
	}
}
