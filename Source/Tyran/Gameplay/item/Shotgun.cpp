// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"




AShotgun::AShotgun()
{
	TimeBetweenShots = 0.8f;
	HitDamage = 20;
	WeaponRange = 10000;
	AllowedViewDotHitDir = 0.8f;
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800;
	TracerRoundInterval = 3;
	Accuracy = 10.0f;
	MagazineSize = 12;
	MagazineCurrent = 12;

	StorageSlot = EInventorySlot::Primary;
	AmmoType = EAmmoType::Shotgun;
	WeaponType = EWeaponType::Shotgun;
	//RifleAttachPoint = TEXT("RifleSocket"); 
}

bool AShotgun::CanFire() const
{
	bool bPawnCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOK = CurrentState == EWeaponState::Idle;
	return bPawnCanFire && bStateOK;
}

void AShotgun::HandleFiring()
{
	if (MagazineCurrent > 0) {
		if (Role == ROLE_Authority) {
			SimulateWeaponFire();
		}
		else
		{
			SimulateWeaponFireServer();
		}
		if (MyPawn && MyPawn->IsLocallyControlled()) {
			FireWeapon();

			// Mettre à jour l'effet sur les objets distants 
			BurstCounter++;
		}
		MagazineCurrent--;
	}
	else
	{
		OnReload();
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

void AShotgun::FireWeapon()
{
	const FVector StartPos = GetMuzzleLocation();

	for (int i = 0; i < 10; ++i) {
		UpdateSpreadVector();
		const FVector AimDir = GetAdjustedAim(); // Obtenir le «ciblage» 
												 //const FVector StartPos = GetCameraDamageStartLocation(AimDir); // Position de départ du tir 
		const FVector EndPos = StartPos + (AimDir * WeaponRange); // Position de fin du tir 

		const FHitResult Impact = WeaponTrace(StartPos, EndPos); // Trouver l'impact 
		ProcessInstantHit(Impact, StartPos, AimDir); // Traiter l'impact
	}
}

void AShotgun::UpdateSpreadVector()
{
	// Dispersion
	float SpreadFactor = 1.0f / Accuracy;
	if (MyPawn->isAiming)
		SpreadFactor /= 2;

	float x = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float y = FMath::RandRange(-SpreadFactor, SpreadFactor);
	float z = FMath::RandRange(-SpreadFactor, SpreadFactor);
	SpreadVector = FVector{ x,y,z };
}
