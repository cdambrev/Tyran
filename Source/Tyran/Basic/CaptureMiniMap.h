// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "CaptureMiniMap.generated.h"


/**
 * 
 */
UCLASS()
class TYRAN_API ACaptureMiniMap : public ASceneCapture2D
{
	GENERATED_BODY()
		UTextureRenderTarget2D * renderTarget;

	

public:
	ACaptureMiniMap();

	UFUNCTION()
	void update();

	TArray<FColor> MapTexData;
	UTexture2D * tex;

	UFUNCTION()
	UTexture2D * GetTextureAtLocation(FVector location);
};
