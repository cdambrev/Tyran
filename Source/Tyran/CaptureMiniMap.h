// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Engine/SceneCapture2D.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include <Image.h>
#include "CaptureMiniMap.generated.h"


/**
 * 
 */
UCLASS()
class TYRAN_API ACaptureMiniMap : public ASceneCapture2D
{
	GENERATED_BODY()
	
	UTextureRenderTarget2D* miniMapRenderTarget;

	UMaterialInstanceDynamic* miniMapMaterial;
	UImage * miniMapImage;
	
public:
	ACaptureMiniMap();
	void setPosition(FVector v);
	UTextureRenderTarget2D * getRenderTarget();
	UImage * getImageMap();
	void BeginPlay();
};
