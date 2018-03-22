// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Loot.h"
#include "UsableObject.h"
#include "UsableObjectLoot.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AUsableObjectLoot : public ALoot
{
	GENERATED_BODY()
	
public:
	AUsableObjectLoot();
	
	/* Classe à ajouter à l'inventaire lorsque l'objet sera ramassé */ 
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<FUsableObject> ObjectClass;
	
	virtual void OnUsed(APawn* InstigatorPawn) override;
};
