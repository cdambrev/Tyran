// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/item/Trap/Trap.h"
#include "Basic/TyranCharacter.h"
#include "Traceur.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ATraceur : public ATrap
{
	GENERATED_BODY()

private:
	ATyranCharacter* cible;
	void triggerDelayed();
public:
	ATraceur();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float visibilityDelay;
	virtual void triggered() override;
	FHitResult TraceurTrace(const FVector & TraceFrom, const FVector & TraceTo) const;
	virtual void Tick(float DeltaTime) override;
	
	virtual FString getType() { return "Traceur"; }
	
};
