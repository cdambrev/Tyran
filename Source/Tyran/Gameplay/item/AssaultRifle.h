// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAssaultRifle : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) 
	FName RifleAttachPoint;

	FVector SpreadVector;
	
	/* Verification d'un «hit»: déclencheur pour 
	le calcul du produit vectoriel entre la 
	direction de vision et la direction du hit */
	UPROPERTY(EditDefaultsOnly) 
	float AllowedViewDotHitDir; 
	
	/* Verification d'un «hit»: échelle de 
	la boite de l'acteur frappé */ 
	UPROPERTY(EditDefaultsOnly) 
	float ClientSideHitLeeway; 
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_HitLocation) 
	FVector HitOriginNotify;

	/* Effet joué lorsqu'une surface est atteinte. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AImpactEffect> ImpactTemplate; 
	
	UPROPERTY(EditDefaultsOnly) 
	FName TrailTargetParam;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailFX;
	
	UPROPERTY(EditDefaultsOnly) 
	UParticleSystem* TracerFX; 
	
	/* Distance minimale pour faire apparaître les traces de balle. */ 
	UPROPERTY(EditDefaultsOnly) 
	float MinimumProjectileSpawnDistance;
	
	UPROPERTY(EditDefaultsOnly)
	int32 TracerRoundInterval; 
	
	/* Nous comptons les coups */
	int32 BulletsShotCount;
	
public:
	AAssaultRifle();	

	virtual void FireWeapon() override;
	FVector GetAdjustedAim() const;
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerNotifyHit(const FHitResult Impact,/* FVector_NetQuantizeNormal Origin,*/ FVector_NetQuantizeNormal ShootDir);
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerNotifyMiss(/*FVector_NetQuantizeNormal Origin, */FVector_NetQuantizeNormal ShootDir);

	void ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	bool ShouldDealDamage(AActor* TestActor) const;
	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	void SimulateInstantHit(const FVector& Origin);
	void SpawnImpactEffects(const FHitResult& Impact);
	void SpawnTrailEffects(const FVector& EndPoint);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override; 
	
	UFUNCTION()
	void OnRep_HitLocation();

protected:
	void UpdateSpreadVector();
};
