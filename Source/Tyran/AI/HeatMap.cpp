// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatMap.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

void HeatMap::update(float deltaTime)
{
/*	map->UpdateResource();
	uint8 * data = new uint8[dimension * dimension * 2]{0};
	map->UpdateTextureRegions(0, 1, &region, dimension * 2, 2, data);*/
}

void HeatMap::init(UWorld * world)
{
	map->UpdateResource();
	uint8 * data = new uint8[dimension * dimension * 2]{ 0 };
	for (int x = 0; x < dimension; ++x) {
		for (int y = 0; y < dimension; ++y) {
			FVector pos((x + 0.5f)*pixelSize - dimension*pixelSize/2, (y + 0.5f)*pixelSize - dimension*pixelSize / 2, -100.0f);
			auto navPos = UNavigationSystem::ProjectPointToNavigation(world, pos);
			if (navPos.Z == pos.Z) { // pas projetable sur le navmesh... ProjectionPointToNavigation renvoit la position de depart
				data[((y * dimension + x) * 2 + 1)] = 255;
			}
			else { // projetable sur le navmesh, navpos est le projete
				data[((y * dimension + x) * 2 + 1)] = 0;
			}
		}
	}
	map->UpdateTextureRegions(0, 1, &region, dimension * 2, 2, data);
}

HeatMap::HeatMap()
{
	dimension = 1024;
	pixelSize = 100;
	region = FUpdateTextureRegion2D(0, 0, 0, 0, dimension, dimension);
	map = UTexture2D::CreateTransient(dimension, dimension, EPixelFormat::PF_R8G8);
}

HeatMap::~HeatMap()
{
}
