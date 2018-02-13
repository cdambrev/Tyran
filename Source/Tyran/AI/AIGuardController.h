// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIGuardController.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API AAIGuardController : public AAIController
{
	GENERATED_BODY()


public:
	AAIGuardController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables custom")
	int nbTargetPoint = 2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables custom")
	float acceptanceRadius = 1000;
	
	/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		void UpdateNextTargetPoint();
	
	/** * Nous v�rifions si le personnage est pr�s et si c'est le cas, nous pla�ons 
	* une r�f�rence dans le BlackBoard. Cette fonction sera utilis�e par le service 
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque 
	* nous approchons de sa zone de patrouille. */ 
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom") 
		void CheckNearbyEnemy();

	/** * Fait en sorte que notre AIEnemyCharacter poursuive le joueur r�f�r� par la cl�
	* TargetActorToFollow du Blackboard. Sera utilis�e dans une t�che du BT pour suivre le personnage
	* principal (le joueur). En sortie, nous retournerons le r�sultat de MoveToActor (Failed,
	* AlreadyAtGoal ou RequestSuccessful) */
	UFUNCTION(BlueprintCallable, Category = "Fonctions custom")
		EPathFollowingRequestResult::Type MoveToEnemy();

};
