// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "CaptureMiniMap.h"
#include "TyranController.generated.h"



/**
 * 
 */
UCLASS()
class TYRAN_API ATyranController : public APlayerController
{
	GENERATED_BODY()
public:
	TSubclassOf<UUserWidget> revolutionnaireUIClass;
	
	UPROPERTY()
	UUserWidget * revUI;
	
	UPROPERTY()
		TSubclassOf<ACaptureMiniMap> defaultCapture;

	UPROPERTY()
		ACaptureMiniMap * captureMap;

private:
	bool updateMapNextTick = true;

public:
	ATyranController();

	UPROPERTY(Transient, Replicated)
	bool isTyran;

	void setTyran(bool b);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void SetPawn(APawn* InPawn) override;

	UFUNCTION(Reliable, Client)
	void initOnTyranClient();

	UFUNCTION(Reliable, Client)
	void initOnRevolutionnaireClient();

	UFUNCTION(Reliable, Client)
	void updateMap();

	UFUNCTION(Reliable, Client)
	void setMapUpdateState(bool updateNextTick);

	void moveMiniMap();

	void Tick(float DeltaSeconds) override;

	void EndOfGame();
};
