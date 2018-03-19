// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RoutineBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API URoutineBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
	/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/* Sera appelée constamment tant que la tâche n'est pas finie (tant que ExecuteTask retourne InProgress) */
	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
	virtual FString GetStaticDescription() const override;
	
	virtual void OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult) override;
	
};
