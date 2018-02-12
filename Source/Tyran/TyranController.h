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

	UPROPERTY(BlueprintReadOnly, Transient, Replicated)
	bool isTyran;

	void setTyran(bool b) {
		isTyran = b;
		if (b) {
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			//InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, onLeftClick);
		}
	}

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	//void onLeftClick();
};
