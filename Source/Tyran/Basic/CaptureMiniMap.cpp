// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureMiniMap.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"


ACaptureMiniMap::ACaptureMiniMap() {
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> mapRender(TEXT("/Game/UI/MapRendering"));
	renderTarget = mapRender.Object;
	
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	SetActorLocation(FVector(0.0f, 0.0f, 2500.0f));
	GetCaptureComponent2D()->SetRelativeLocation(FVector(0.0f, 0.0f, 2500.0f));
	GetCaptureComponent2D()->TextureTarget = renderTarget;

	tex = UTexture2D::CreateTransient(128,128);
	
}

void ACaptureMiniMap::update() {
	GetCaptureComponent2D()->CaptureScene();
	
	auto t = renderTarget->GameThread_GetRenderTargetResource();
	t->ReadPixels(MapTexData);
	for (int i = 0; i < MapTexData.Num(); ++i) {
		FColor c = MapTexData[i];
		FColor res = FColor(0, 0, 0);
		float z = c.R / 255.0f + c.G / 65025.0f + c.B / 16581375.0f + c.A / 4228250625.0f; // Formule pour avoir le Z buffer à partir du RGBA
		if (z < 0.085f) {
			if (z > 0.001f)
				res = FColor(0, 0, 128);
			else
				res = FColor(0, 0, 255);
		}

		MapTexData[i] = res;

	}

}

UTexture2D * ACaptureMiniMap::GetTextureAtLocation(FVector location)
{
	int tailleX = tex->GetSizeX();
	int tailleY = tex->GetSizeY();

	int texY = (-location.X * 2048.0 / 250000.0) + 1024;
	int texX = (location.Y * 2048.0 / 250000.0) + 1024;

	TArray<FColor> MapTexDataResize;
	for (int y = texY - tailleY / 2; y < texY - tailleY / 2 + tailleY; ++y) {
		int x = texX - tailleX / 2;
		MapTexDataResize.Append(&MapTexData[x + y * 2048], tailleY);
	}

	for (int y = tailleY / 2 - 1; y <= tailleY / 2 + 1; ++y) {
		for (int x = tailleX / 2 - 1; x <= tailleX / 2 + 1; ++x) {
			MapTexDataResize[x + y * tailleY] = FColor(0, 255, 0);
		}
	}

	
	FTexture2DMipMap& Mip = tex->PlatformData->Mips[0];
	void * Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, MapTexDataResize.GetData(), tailleX*tailleY*4);
	Mip.BulkData.Unlock();
	
	tex->UpdateResource();


	return tex;
}

