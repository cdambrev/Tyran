// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerPlayerState.h"

void AManagerPlayerState::addMoneyIncome(float amount)
{
	moneyIncomPerSecond += amount;
}

void AManagerPlayerState::addMaxPopulation(int amount)
{
	maxPopulation += amount;
}

bool AManagerPlayerState::reservePopulationSpace(int amount)
{
	if (population+amount <= maxPopulation) {
		population += amount;
		return true;
	}
	else {
		return false;
	}
}

bool AManagerPlayerState::spendMoney(float amount)
{
	if (money - amount >= 0.0f) {
		money -= amount;
		return true;
	}
	else {
		return false;
	}
}

void AManagerPlayerState::Tick(float DeltaTime)
{
	money += moneyIncomPerSecond*DeltaTime;
}

void AManagerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AManagerPlayerState, money);
	DOREPLIFETIME(AManagerPlayerState, population);
	DOREPLIFETIME(AManagerPlayerState, maxPopulation);
	DOREPLIFETIME(AManagerPlayerState, moneyIncomPerSecond);
}


