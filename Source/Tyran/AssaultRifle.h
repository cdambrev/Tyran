// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// *** Variables pour tir et dommages 
	UPROPERTY(EditDefaultsOnly) 
	float HitDamage; 
	
	UPROPERTY(EditDefaultsOnly) 
	TSubclassOf<class UDamageType> DamageType; 
	
	UPROPERTY(EditDefaultsOnly) 
	float WeaponRange; 
	
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
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;
	
public:
	AAssaultRifle();	

	virtual void FireWeapon() override;
	FVector GetAdjustedAim() const;
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;
	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerNotifyHit(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir);
	UFUNCTION(Reliable, Server, WithValidation) 
	void ServerNotifyMiss(FVector_NetQuantizeNormal ShootDir);
	void ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);
	bool ShouldDealDamage(AActor* TestActor) const;
	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override; 
	virtual void OnEquipFinished() override; 
	virtual void OnUnEquip() override; 
	virtual void OnEnterInventory(ATyranCharacter* NewOwner) override; 
	void UpdateRifle(bool Enabled); 
	
	UFUNCTION()
	void OnRep_HitLocation();
	UFUNCTION() 		
	void OnRep_IsActive();
};
