// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveToEnemyBTTaskNode.h"
#include "AI/AIGuardController.h" 

UMoveToEnemyBTTaskNode::UMoveToEnemyBTTaskNode() { 
	// Le nom que prendra le noeud dans le BT 
	NodeName = "MoveToEnemy"; 
	// Cette t�che appelle TickTask 
	bNotifyTick = true; 
}

/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */ 
EBTNodeResult::Type UMoveToEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) { 
	
	// Obtenir un pointeur sur notre AIEnemyController 
	AAIGuardController* AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner()); 
	
	// Nous pr�parons le r�sultat de la t�che. Elle doit retourner InProgress 
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	
	// Appel de la fonction MoveToEnemy du contr�leur, nous pla�ons le r�sultat dans MoveToActorResult 
	EPathFollowingRequestResult::Type MoveToActorResult = AIGuardController->MoveToEnemy(); 
	
	// Si nous sommes d�j� aupr�s du personnage. Nous retournons Succeeded. 
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal) { 
		NodeResult = EBTNodeResult::Succeeded; 
	} 

	return NodeResult; 
}

/* Sera appel�e constamment tant que la t�che n'est pas finie (tant que ExecuteTask retourne InProgress) */
void UMoveToEnemyBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) { 
	
	// Obtenir un pointeur sur notre AIEnemyController 
	AAIGuardController* AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner()); 
	
	// Appeler la fonction MoveToEnemy du contr�leur et nous conservons le r�sultat 
	// en MoveToActorResult
	EPathFollowingRequestResult::Type MoveToActorResult = AIGuardController->MoveToEnemy(); 

	// Si nous sommes rendu sur l'objectif, nous terminons la t�che avec succ�s 
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal) { 
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
	} 
} 

/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */ 
FString UMoveToEnemyBTTaskNode::GetStaticDescription() const { 
	return TEXT("Poursuite du personnage principal"); 
}
