// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GUIComponent.generated.h"


UCLASS()
class TYRAN_API UGUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Reliable, Client)
		virtual void initAllGui();

	UFUNCTION(Reliable, Client)
		virtual void displayDefaultGUI();

	UFUNCTION(Reliable, Client)
		virtual void removeDefaultGUI();

	UFUNCTION(Reliable, Client)
		virtual void displayVictoryGUI();

	UFUNCTION(Reliable, Client)
		virtual void removeVictoryGUI();
	
	UFUNCTION(Reliable, Client)
		virtual void displayDefeatGUI();
	
	UFUNCTION(Reliable, Client)
		virtual void removeDefeatGUI();
};
