// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIGuardTargetPoint.h"
#include <Array.h>
#include "AIGuardController.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAIGuardController : public AAIController
{
	GENERATED_BODY()

private:
	TArray<AAIGuardTargetPoint*> patrolPoints;



public:
	AAIGuardController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables custom")
		int nbTargetPoint = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables custom")
		float acceptanceRadius = 1000;

	/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		void UpdateNextTargetPoint();

	/** * Nous vérifions si le personnage est près et si c'est le cas, nous plaçons
	* une référence dans le BlackBoard. Cette fonction sera utilisée par le service
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
	* nous approchons de sa zone de patrouille. */
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		void CheckNearbyEnemy();

	/** * Fait en sorte que notre AIEnemyCharacter poursuive le joueur référé par la clé
	* TargetActorToFollow du Blackboard. Sera utilisée dans une tâche du BT pour suivre le personnage
	* principal (le joueur). En sortie, nous retournerons le résultat de MoveToActor (Failed,
	* AlreadyAtGoal ou RequestSuccessful) */
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		EPathFollowingRequestResult::Type MoveToEnemy();

	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		void setPatrolPoint(TArray<AAIGuardTargetPoint*> patrolPoints_);

	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		TArray<AAIGuardTargetPoint*> getPatrolPoints();

	void enterFight();

	void exitFight();

	bool isFighting();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool fighting = false;

	UPROPERTY()
	class ASquad * squadPtr;

	UPROPERTY()
	TSubclassOf<ASquad> defaultSquad;
};