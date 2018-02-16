// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TyranGUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TYRAN_API UTyranGUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	TSubclassOf<UUserWidget> managerUIClass;
	TSubclassOf<UUserWidget> guardUIClass;
	UUserWidget* guardOrderWidget;
	
	// Sets default values for this component's properties
	UTyranGUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(Reliable, Client)
	void guardUI(FVector2D mouseLocation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	
};
