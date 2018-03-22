// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AI/AIGuardController.h"
#include "Squad.generated.h"

UCLASS()
class TYRAN_API ASquad : public APawn
{
	GENERATED_BODY()


private:
	TArray<AAIGuardController*> gardes;

public:
	// Sets default values for this pawn's properties
	ASquad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void addGarde(AAIGuardController & garde);
	void removeGarde(AAIGuardController & garde);

	bool empty();

	TArray<AAIGuardController*> & getGardes();
	
};
