// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ManagerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AManagerPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Replicated)
	float money;
	UPROPERTY(BlueprintReadOnly,Replicated)
	int population;
	UPROPERTY(BlueprintReadOnly,Replicated)
	int maxPopulation;
	UPROPERTY(BlueprintReadOnly,Replicated)
	float moneyIncomPerSecond;

	AManagerPlayerState() {
		money = 600.0f;
		population = 0;
		maxPopulation = 0;
		moneyIncomPerSecond = 1.0f;
		PrimaryActorTick.bCanEverTick = true;
	}

	//Call on server
	void addMoneyIncome(float amount);
	//Call on server
	void addMaxPopulation(int amount);

	//Call on server
	bool reservePopulationSpace(int amount);
	//Call on server
	bool spendMoney(float amount);

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
