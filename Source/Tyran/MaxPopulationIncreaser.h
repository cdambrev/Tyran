// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "MaxPopulationIncreaser.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TYRAN_API UMaxPopulationIncreaser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMaxPopulationIncreaser();

	//UFUNCTION(Reliable, Server, WithValidation)
	//void setPopulationAmount(int amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int populationAmount;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
