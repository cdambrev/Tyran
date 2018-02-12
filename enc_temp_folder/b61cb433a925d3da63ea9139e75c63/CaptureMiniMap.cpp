// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureMiniMap.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ACaptureMiniMap::ACaptureMiniMap() {

	USceneCaptureComponent2D * captureComponent = GetCaptureComponent2D();
	captureComponent->bCaptureEveryFrame = true;
	//captureComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
//	miniMapRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("MiniMapRenderTarget"));
//	miniMapRenderTarget->InitAutoFormat(1024, 1024);
//	miniMapRenderTarget->AddressX = TA_Wrap;
//	miniMapRenderTarget->AddressY = TA_Wrap;
	
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> textureTargetObj(TEXT("/Game/Blueprints/MapRendering"));
	if (textureTargetObj.Object != NULL) {
		captureComponent->TextureTarget = textureTargetObj.Object;
	}
	
	captureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	captureComponent->OrthoWidth = 3500;
	captureComponent->bAbsoluteLocation = true;
	captureComponent->SetRelativeRotation(FQuat(FVector(0.0, 1.0, 0.0), 1.5707));
	captureComponent->SetRelativeLocation(FVector(100,500,0));
	captureComponent->CaptureSource = ESceneCaptureSource::SCS_DeviceDepth;

	
}

void ACaptureMiniMap::setPosition(FVector v) {
	SetActorLocation(v);
	GetCaptureComponent2D()->SetRelativeLocation(v);
	GetCaptureComponent2D()->UpdateContent();
}

void ACaptureMiniMap::BeginPlay() {

	Super::BeginPlay();

	GetCaptureComponent2D()->UpdateContent();
//	miniMapDynamicMaterial->SetTextureParameterValue("MiniMap_Mat", miniMapRenderTarget);


}

UTextureRenderTarget2D * ACaptureMiniMap::getRenderTarget() {
	return miniMapRenderTarget;
}