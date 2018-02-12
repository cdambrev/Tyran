// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.h"
#include "BuildingSlot.generated.h"

UCLASS()
class TYRAN_API ABuildingSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuilding> defaultBuilding;

	UPROPERTY(BlueprintReadOnly)
	ABuilding * currBuilding;

	void build(TSubclassOf<ABuilding> type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
