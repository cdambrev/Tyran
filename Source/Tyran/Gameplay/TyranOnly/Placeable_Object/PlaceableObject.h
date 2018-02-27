// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Basic/TyranCharacter.h"
#include "PlaceableObject.generated.h"

UCLASS()
class TYRAN_API APlaceableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObject();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float basePrice;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FString getType() { return "PlaceableObject"; }

	
};
