// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingHint.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
ABuildingHint::ABuildingHint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Game/Objects/BuildingHints/HintMaterial"));
	hintMaterial = MatFinder.Object;

	isColliding = false;
	isGreen = true;
}

// Called when the game starts or when spawned
void ABuildingHint::BeginPlay()
{
	Super::BeginPlay();
	isGreen = true;
	hintMaterialDyn = UMaterialInstanceDynamic::Create(hintMaterial, this);
	auto comp = GetComponents();
	for (auto c : comp)
	{
		UStaticMeshComponent * pMeshComponent = static_cast<UStaticMeshComponent *>(c);
		if (pMeshComponent)
		{
			for (int i = 0; i < pMeshComponent->GetNumMaterials(); ++i)
			{
				pMeshComponent->SetMaterial(i, hintMaterialDyn);
			}
		}
	}
}

// Called every frame
void ABuildingHint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingHint::setValidPosition(bool b)
{
	if (b && !isGreen) {
		hintMaterialDyn->SetVectorParameterValue(FName{ "Value" }, FLinearColor(0, 1, 0, 1));
		isGreen = true;
	}
	else if(!b && isGreen){
		hintMaterialDyn->SetVectorParameterValue(FName{ "Value" }, FLinearColor(1, 0, 0, 1));
		isGreen = false;
	}
}

bool ABuildingHint::checkValidPosition()
{
	setValidPosition(!isColliding);
	return !isColliding;
}
