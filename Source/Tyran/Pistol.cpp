// Fill out your copyright notice in the Description page of Project Settings.

#include "Pistol.h"

APistol::APistol()
{
	TimeBetweenShots = 0.2f;
	HitDamage = 26;
	WeaponRange = 10000;
	AllowedViewDotHitDir = 0.8f;
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800;
	TracerRoundInterval = 3;
	Accuracy = 40.0f;
	MagazineSize = 12;
	MagazineCurrent = 12;

	StorageSlot = EInventorySlot::Secondary;
	AmmoType = EAmmoType::Pistol;
	WeaponType = EWeaponType::Pistol;
	//RifleAttachPoint = TEXT("RifleSocket"); 
}

void APistol::FireWeapon()
{
	UpdateSpreadVector();
	const FVector AimDir = GetAdjustedAim(); // Obtenir le «ciblage» 
											 //const FVector StartPos = GetCameraDamageStartLocation(AimDir); // Position de départ du tir 
	const FVector StartPos = GetMuzzleLocation();
	const FVector EndPos = StartPos + (AimDir * WeaponRange); // Position de fin du tir 

	const FHitResult Impact = WeaponTrace(StartPos, EndPos); // Trouver l'impact 
	ProcessInstantHit(Impact, StartPos, AimDir); // Traiter l'impact
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
}


