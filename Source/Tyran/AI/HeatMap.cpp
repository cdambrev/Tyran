// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatMap.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

void HeatMap::update(float deltaTime)
{
	int sideMultiplicator = 500 * deltaTime;
	int sideDivMultiplicator = 500 * deltaTime;
	FTexture2DMipMap * mipMap = &map->PlatformData->Mips[0];
	FByteBulkData* rawImageData = &mipMap->BulkData;
	uint16* result = static_cast<uint16*>(rawImageData->Lock(LOCK_READ_WRITE));
	uint16* data = new uint16[dimension * dimension * 4];
	FMemory::Memcpy(data, result, dimension * dimension * 4);
	/*for (int y = 1; y < dimension - 1; ++y) {
		for (int x = 1; x < dimension - 1; ++x) {
			if (data[((y * dimension + x) * 2 + 1)] == 0) {
				int divisor = 2;
				int total = (2*data[((y * dimension + x) * 2 + 0)]);
				if (data[(((y + 1) * dimension + (x)) * 2 + 1)] == 0 && data[(((y + 1) * dimension + (x)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y + 1) * dimension + (x)) * 2 + 0)] < maxPropagation) {
						total += data[(((y + 1) * dimension + (x)) * 2 + 0)] * sideMultiplicator;
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y) * dimension + (x-1)) * 2 + 1)] == 0 && data[(((y) * dimension + (x-1)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y)* dimension + (x - 1)) * 2 + 0)] < maxPropagation) {
						total += data[(((y)* dimension + (x - 1)) * 2 + 0)] * sideMultiplicator;
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y) * dimension + (x+1)) * 2 + 1)] == 0 && data[(((y) * dimension + (x+1)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y)* dimension + (x + 1)) * 2 + 0)] < maxPropagation) {
						total += data[(((y)* dimension + (x + 1)) * 2 + 0)] * sideMultiplicator;
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				if (data[(((y -1)* dimension + (x)) * 2 + 1)] == 0 && data[(((y-1) * dimension + (x)) * 2 + 0)] > data[((y * dimension + x) * 2 + 0)]) {
					if (data[(((y - 1)* dimension + (x)) * 2 + 0)] < maxPropagation) {
						total += data[(((y - 1)* dimension + (x)) * 2 + 0)] * sideMultiplicator;
					}
					else {
						total += sideMultiplicator*maxPropagation;
					}
					divisor += sideDivMultiplicator;
				}
				result[((y * dimension + x) * 2 + 0)] = (total / (divisor));
				//if (result[((y * dimension + x) * 2 + 0)] >= 200*deltaTime) {
				result[((y * dimension + x) * 2 + 0)] *= 1 - 0.03*deltaTime;
				//}
			}
		}
	}*/

	/*deltaTime = 1;
	for (int y = 1; y < dimension - 1; ++y) {
		for (int x = 1; x < dimension - 1; ++x) {
			if (data[((y * dimension + x) * 2 + 1)] == 0) {
				int total = 16* data[((y * dimension + x) * 2 + 0)];
				int div = 16;
				for (int i = -5; i <= 5; ++i) {
					if (y + i > 0 && y + i < dimension && data[(((y + i) * dimension + (x)) * 2 + 1)] == 0 && data[(((y + i) * dimension + (x)) * 2 + 0)] >= data[((y * dimension + x) * 2 + 0)]) {
						total += convolution[i + 5] * data[(((y + i) * dimension + (x)) * 2 + 0)] * deltaTime;
						div += convolution[i + 5] * deltaTime;
					}
				}
				result[((y * dimension + x) * 2 + 0)] = total / div;
			}
		}
	}
	FMemory::Memcpy(data, result, dimension * dimension * 4);
	for (int y = 1; y < dimension - 1; ++y) {
		for (int x = 1; x < dimension - 1; ++x) {
			if (data[((y * dimension + x) * 2 + 1)] == 0) {
				int total = 16 * data[((y * dimension + x) * 2 + 0)];
				int div = 16;
				for (int i = -5; i <= 5; ++i) {
					if (x + i > 0 && x + i < dimension && data[(((y)* dimension + (x + i)) * 2 + 1)] == 0 && data[(((y)* dimension + (x + i)) * 2 + 0)] >= data[((y * dimension + x) * 2 + 0)]) {
						total += convolution[i + 5] * data[(((y) * dimension + (x + i)) * 2 + 0)] * deltaTime;
						div += convolution[i + 5] * deltaTime;
					}
				}
				result[((y * dimension + x) * 2 + 0)] = total / div;
				result[((y * dimension + x) * 2 + 0)] *= (1 - 0.03*deltaTime);
			}
		}
	}*/
	float s2 = sqrt(2);

	float * convolution = new float[11 * 11]{
		0,0,0,3-2*s2,2-s2,1,2-s2,3-2*s2,0,0,0,
		0,6-4*s2,5-3*s2,4-2*s2,3-s2,2,3-s2,4-2*s2,5-3*s2,6-4*s2,0,
		0,5-3*s2,6-3*s2,5-2*s2,4-s2,3,4-s2,5-2*s2,6-3*s2,5-3*s2,0,
		3-2*s2,4-2*s2,5-2*s2,6-2*s2,5-s2,4,5-s2,6-2*s2,5-2*s2,4-2*s2,3-2*s2,
		2-s2,3-s2,4-s2,5-s2,6-s2,5,6-s2,5-s2,4-s2,3-s2,2-s2,
		1,2,3,4,5,6,5,4,3,2,1,
		2 - s2,3 - s2,4 - s2,5 - s2,6 - s2,5,6 - s2,5 - s2,4 - s2,3 - s2,2 - s2,
		3 - 2 * s2,4 - 2 * s2,5 - 2 * s2,6 - 2 * s2,5 - s2,4,5 - s2,6 - 2 * s2,5 - 2 * s2,4 - 2 * s2,3 - 2 * s2,
		0,5 - 3 * s2,6 - 3 * s2,5 - 2 * s2,4 - s2,3,4 - s2,5 - 2 * s2,6 - 3 * s2,5 - 3 * s2,0,
		0,6 - 4 * s2,5 - 3 * s2,4 - 2 * s2,3 - s2,2,3 - s2,4 - 2 * s2,5 - 3 * s2,6 - 4 * s2,0,
		0,0,0,3 - 2 * s2,2 - s2,1,2 - s2,3 - 2 * s2,0,0,0
	};

	//deltaTime = 1;
	for (int y = 1; y < dimension - 1; ++y) {
		for (int x = 1; x < dimension - 1; ++x) {
			if (data[((y * dimension + x) * 2 + 1)] == 0) {
				float total = 16.0f * data[((y * dimension + x) * 2 + 0)];
				float div = 16.0f;
				for (int i = -5; i <= 5; ++i) {
					for (int j = -5; j <= 5; ++j) {
						if (y + i > 0 && y + i < dimension && x + j > 0 && x + j < dimension && data[(((y + i) * dimension + (x + j)) * 2 + 1)] == 0 && data[(((y + i) * dimension + (x + j)) * 2 + 0)] >= data[((y * dimension + x) * 2 + 0)]) {
							total += convolution[(i + 5) * 11 + j + 5] * data[(((y + i) * dimension + (x + j)) * 2 + 0)] * deltaTime;
							div += convolution[(i + 5) * 11 + j + 5] * deltaTime;
						}
					}
				}
				result[((y * dimension + x) * 2 + 0)] = rint(total / div);
				result[((y * dimension + x) * 2 + 0)] *= (1 - 0.03*deltaTime);
			}
		}
	}

	rawImageData->Unlock();
	map->UpdateResource();
	delete data;
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
