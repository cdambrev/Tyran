// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatMap.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

void HeatMap::update(float deltaTime)
{
	int sideMultiplicator = 1;
	int sideDivMultiplicator = 1;
	FTexture2DMipMap * mipMap = &map->PlatformData->Mips[0];
	FByteBulkData* rawImageData = &mipMap->BulkData;
	uint16* data = static_cast<uint16*>(rawImageData->Lock(LOCK_READ_WRITE));
	for (int y = 1; y < dimension - 1; ++y) {
		for (int x = 1; x < dimension - 1; ++x) {
			if (data[((y * dimension + x) * 2 + 1)] == 0) {
				int divisor = 2;
				int total = 2*data[((y * dimension + x) * 2 + 0)];
				if (data[(((y + 1) * dimension + (x)) * 2 + 1)] == 0 && data[(((y + 1) * dimension + (x)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y + 1) * dimension + (x)) * 2 + 0)] < maxPropagation) {
						total += data[(((y + 1) * dimension + (x)) * 2 + 0)];
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y) * dimension + (x-1)) * 2 + 1)] == 0 && data[(((y) * dimension + (x-1)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y)* dimension + (x - 1)) * 2 + 0)] < maxPropagation) {
						total += data[(((y)* dimension + (x - 1)) * 2 + 0)];
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y) * dimension + (x+1)) * 2 + 1)] == 0 && data[(((y) * dimension + (x+1)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y)* dimension + (x + 1)) * 2 + 0)] < maxPropagation) {
						total += data[(((y)* dimension + (x + 1)) * 2 + 0)];
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y -1)* dimension + (x)) * 2 + 1)] == 0 && data[(((y-1) * dimension + (x)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y - 1)* dimension + (x)) * 2 + 0)] < maxPropagation) {
						total += data[(((y - 1)* dimension + (x)) * 2 + 0)];
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				data[((y * dimension + x) * 2 + 0)] = total / divisor;
				if (data[((y * dimension + x) * 2 + 0)] >= 50) {
					data[((y * dimension + x) * 2 + 0)] -= 50;
				}
			}
		}
	}
	rawImageData->Unlock();
	map->UpdateResource();
}

void HeatMap::addHeatSource(FVector2D pos)
{
	setHeatAtPoint(pos, maxSource);
}

void HeatMap::setHeatAtPoint(FVector2D position, uint16 val)
{
	int x = rint(position.X / pixelSize + dimension / 2);
	int y = rint(position.Y / pixelSize + dimension / 2);
	FTexture2DMipMap * mipMap = &map->PlatformData->Mips[0];
	FByteBulkData* rawImageData = &mipMap->BulkData;
	uint16* data = static_cast<uint16*>(rawImageData->Lock(LOCK_READ_WRITE));
	data[((y * dimension + x) * 2 + 0)] = val;
	rawImageData->Unlock();
	map->UpdateResource();
}

void HeatMap::init(UWorld * world)
{
	FTexture2DMipMap * mipMap = &map->PlatformData->Mips[0];
	FByteBulkData* rawImageData = &mipMap->BulkData;
	uint16* data = static_cast<uint16*>(rawImageData->Lock(LOCK_READ_WRITE));
	for (int x = 0; x < dimension; ++x) {
		for (int y = 0; y < dimension; ++y) {
			FVector pos((x + 0.5f)*pixelSize - dimension*pixelSize/2, (y + 0.5f)*pixelSize - dimension*pixelSize / 2, -100.0f);
			auto navPos = UNavigationSystem::ProjectPointToNavigation(world, pos);
			if (navPos.Z == pos.Z) { // pas projetable sur le navmesh... ProjectionPointToNavigation renvoit la position de depart
				data[((y * dimension + x) * 2 + 1)] = 65535;
			}
			else { // projetable sur le navmesh, navpos est le projete
				data[((y * dimension + x) * 2 + 1)] = 0;
			}
			data[((y * dimension + x) * 2 + 0)] = 0;
		}
	}
	rawImageData->Unlock();
	map->UpdateResource();
}

HeatMap::HeatMap()
{
	dimension = 1024;
	pixelSize = 100;
	region = FUpdateTextureRegion2D(0, 0, 0, 0, dimension, dimension);
	map = UTexture2D::CreateTransient(dimension, dimension, EPixelFormat::PF_G16R16);
}

HeatMap::~HeatMap()
{
}
