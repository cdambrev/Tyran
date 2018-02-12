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

	/* Fonction d'ex�cution de la t�che, cette t�che devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 *NodeMemory) override;

	/** Permet de d�finir une description pour la t�che. C'est ce texte qui appara�tra dans le noeud que nous ajouterons au Behavior Tree */
	virtual FString GetStaticDescription() const override;
	
	
	
};
