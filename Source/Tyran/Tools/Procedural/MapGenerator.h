// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerationBrick.h"
#include "MapGenerator.generated.h"

UCLASS()
class TYRAN_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	struct MergeTemplate {
		TSubclassOf<AActor> source1;
		TSubclassOf<AActor> source2;
		TSubclassOf<AActor> resultBuild;
		MergeTemplate(TSubclassOf<AActor> s1, TSubclassOf<AActor> s2, TSubclassOf<AActor> rB) {
			source1 = s1;
			source2 = s2;
			resultBuild = rB;
		}
	};
	// Sets default values for this actor's properties
	AMapGenerator();
	~AMapGenerator();
	TArray<GenerationBrick *> Defaultbricks;
	TArray<GenerationBrick> bricks;
	TArray<GenerationBrick> tempBricks;
	TArray<MergeTemplate> mergeTemplates;
	GenerationBrick startPoint;
	int blockSize;
	int nbIterations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
