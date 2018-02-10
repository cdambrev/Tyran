// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "CaptureMiniMap.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ACaptureMiniMap : public ASceneCapture2D
{
	GENERATED_BODY()
	
	
public:
	
	void setPosition(FVector v);
};
