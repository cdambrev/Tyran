// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveToEnemyBTTaskNode.h"
#include "AI/AIGuardController.h" 

UMoveToEnemyBTTaskNode::UMoveToEnemyBTTaskNode() { 
	// Le nom que prendra le noeud dans le BT 
	NodeName = "MoveToEnemy"; 
	// Cette tâche appelle TickTask 
	bNotifyTick = true; 
}

/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */ 
EBTNodeResult::Type UMoveToEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) { 
	
	// Obtenir un pointeur sur notre AIEnemyController 
	AAIGuardController* AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner()); 
	
	// Nous préparons le résultat de la tâche. Elle doit retourner InProgress 
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	
	// Appel de la fonction MoveToEnemy du contrôleur, nous plaçons le résultat dans MoveToActorResult 
	EPathFollowingRequestResult::Type MoveToActorResult = AIGuardController->MoveToEnemy(); 
	
	// Si nous sommes déjà auprès du personnage. Nous retournons Succeeded. 
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal) { 
		NodeResult = EBTNodeResult::Succeeded; 
	} 

	return NodeResult; 
}

/* Sera appelée constamment tant que la tâche n'est pas finie (tant que ExecuteTask retourne InProgress) */
void UMoveToEnemyBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) { 
	
	// Obtenir un pointeur sur notre AIEnemyController 
	AAIGuardController* AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner()); 
	
	// Appeler la fonction MoveToEnemy du contrôleur et nous conservons le résultat 
	// en MoveToActorResult
	EPathFollowingRequestResult::Type MoveToActorResult = AIGuardController->MoveToEnemy(); 

	// Si nous sommes rendu sur l'objectif, nous terminons la tâche avec succès 
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal) { 
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
	} 
} 

/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */ 
FString UMoveToEnemyBTTaskNode::GetStaticDescription() const { 
	return TEXT("Poursuite du personnage principal"); 
}
