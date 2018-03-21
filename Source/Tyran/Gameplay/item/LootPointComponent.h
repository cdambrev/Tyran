// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LootPointComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TYRAN_API ULootPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULootPointComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int spawnValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawnZoneRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
};
