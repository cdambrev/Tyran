// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <UserWidget.h>
#include "TyranHUD.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API ATyranHUD : public AHUD
{
	GENERATED_BODY()
private:
	// widget par défaut
	UUserWidget* defaultUIWidget;
	TSubclassOf<UUserWidget> defaultUIClass;

	// widget pour les ordres des gardes
	TSubclassOf<UUserWidget> guardUIClass;

	// widget pour les patrolPoints
	UUserWidget* patrolPointsMode;
	TSubclassOf<UUserWidget> patrolPointsModeUIClass;

	// widget pour le timer
	UUserWidget* timerUIWidget;
	TSubclassOf<UUserWidget> timerUIClass;

	// widget pour la fin du jeu
	UUserWidget* endOfGameUIWidget;
	TSubclassOf<UUserWidget> endOfGameUIClass;

	// pour les infos sur les gardes
	TSubclassOf<UUserWidget> guardInfoUIClass;

protected:	
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* guardOrderWidget;
	
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* guardInfo;

public:

	ATyranHUD();

	void BeginPlay() override;

	void displayGuardOrder(FVector2D position);

	void removeGuardOrder();

	void displayPatrolPointsMode();

	void removePatrolPointsMode();

	void displayGuardInfo();

	void removeGuardInfo();

	void displayDefaultUI();

	void removeDefaultUI();

	void modificationGuardInfoOffensif();
	void modificationGuardInfoDefensif();
	void modificationGuardInfoTenirPos();
	void modificationGuardInfoFuite();
	
	// Create Event
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnVisibleGuardOrder();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnVisibleGuardInfo();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnModificationGuardInfoOffensif();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnModificationGuardInfoDefensif();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnModificationGuardInfoTenirPos();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void OnModificationGuardInfoFuite();

	void OnEndOfGame();

	void removeAllPermanently();
};
