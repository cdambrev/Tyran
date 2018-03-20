// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	TimeBetweenShots = 0.1f;
	HitDamage = 26; 
	WeaponRange = 10000; 
	AllowedViewDotHitDir = 0.8f; 
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800; 
	TracerRoundInterval = 3;
	Accuracy = 20.0f;
	MagazineSize = 30;
	MagazineCurrent = 30;

	StorageSlot = EInventorySlot::Primary; 
	AmmoType = EAmmoType::AssaultRifle;
	WeaponType = EWeaponType::AssaultRifle;
	//RifleAttachPoint = TEXT("RifleSocket"); 
}

void AAssaultRifle::FireWeapon()
{
	UpdateSpreadVector();
	const FVector AimDir = GetAdjustedAim(); // Obtenir le «ciblage» 
	//const FVector StartPos = GetCameraDamageStartLocation(AimDir); // Position de départ du tir 
	const FVector StartPos = GetMuzzleLocation();
	const FVector EndPos = StartPos + (AimDir * WeaponRange); // Position de fin du tir 
	
	const FHitResult Impact = WeaponTrace(StartPos, EndPos); // Trouver l'impact 
	ProcessInstantHit(Impact, StartPos, AimDir); // Traiter l'impact
	//MagazineCurrent--;
}

void AAssaultRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	//DOREPLIFETIME_CONDITION(AAssaultRifle, HitOriginNotify, COND_SkipOwner);
}
