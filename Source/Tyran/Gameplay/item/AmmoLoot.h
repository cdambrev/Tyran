// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Loot.h"
#include "Basic/Enum/TyranTypes.h"
#include "AmmoLoot.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAmmoLoot : public ALoot
{
	GENERATED_BODY()
	
public:
	AAmmoLoot();

	/* Classe à ajouter à l'inventaire lorsque l'objet sera ramassé */
	UPROPERTY(EditDefaultsOnly)
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly)
	int nbAmmo;

	virtual void OnUsed(APawn* InstigatorPawn) override;
	
	
};
