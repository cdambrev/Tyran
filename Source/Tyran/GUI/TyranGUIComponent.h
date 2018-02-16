// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUIComponent.h"
#include "Net/UnrealNetwork.h"
#include <UserWidget.h>
#include "TyranGUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TYRAN_API UTyranGUIComponent : public UGUIComponent
{
	GENERATED_BODY()

public:	
	TSubclassOf<UUserWidget> managerUIClass;
	TSubclassOf<UUserWidget> guardUIClass;
	UUserWidget* defaultGUI;
	UUserWidget* guardOrderWidget;
	
	// Sets default values for this component's properties
	UTyranGUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void initAllGui_Implementation() override;

	virtual void displayDefaultGUI_Implementation() override;

	virtual void removeDefaultGUI_Implementation() override;

	virtual void displayVictoryGUI_Implementation() override;

	virtual void removeVictoryGUI_Implementation() override;

	virtual void displayDefeatGUI_Implementation() override;

	virtual void removeDefeatGUI_Implementation() override;
	
	UFUNCTION(Reliable, Client)
		void displayGuardUI(FVector2D mouseLocation);
	
	UFUNCTION(Reliable, Client)
		void removeGuardUI();

	bool isGuardUIDisplayed();
};
