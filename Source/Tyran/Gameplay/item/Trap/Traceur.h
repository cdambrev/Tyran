// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Trap/Trap.h"
#include "Traceur.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ATraceur : public ATrap
{
	GENERATED_BODY()

public:
	ATraceur();
	virtual void triggered() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual FString getType() { return "Traceur"; }
	
};
