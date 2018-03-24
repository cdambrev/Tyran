// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingHint.generated.h"

UCLASS()
class TYRAN_API ABuildingHint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingHint();

	UPROPERTY()
	UMaterialInterface * hintMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic * hintMaterialDyn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isColliding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isGreen;

	UPROPERTY(EditAnywhere)
	bool groundPlaceable;

	UPROPERTY(EditAnywhere)
	bool wallPlaceable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void setValidPosition(bool b);
	bool checkValidPosition();
	
};
