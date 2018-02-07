// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpdateNextTargetPointBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class TYRAN_API UUpdateNextTargetPointBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UUpdateNextTargetPointBTTaskNode();

	/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 *NodeMemory) override;

	/** Permet de définir une description pour la tâche. C'est ce texte qui apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
	virtual FString GetStaticDescription() const override;
	
	
	
};
