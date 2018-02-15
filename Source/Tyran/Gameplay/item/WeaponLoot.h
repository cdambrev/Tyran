// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Loot.h"
#include "Weapon.h"
#include "Basic/TyranCharacter.h"
#include "WeaponLoot.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AWeaponLoot : public ALoot
{
	GENERATED_BODY()
	
public:
	AWeaponLoot();

	/* Classe à ajouter à l'inventaire lorsque l'objet sera ramassé */ 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;
	
	virtual void OnUsed(APawn* InstigatorPawn) override;
};
