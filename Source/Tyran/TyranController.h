// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "TyranController.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ATyranController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATyranController();

	UPROPERTY(Transient, Replicated)
	bool isTyran;

	void setTyran(bool b) {
		isTyran = b;
	}

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void BeginPlay() override;
};
