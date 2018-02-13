// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateNextTargetPointBTTaskNode.h"
#include "AIGuardController.h"

//Constructeur de la classe
UUpdateNextTargetPointBTTaskNode::UUpdateNextTargetPointBTTaskNode() {
	// Nous definissons le nom aue portera le noeud dans le BT
	NodeName = "UpdateNextTargetPoint";
}

/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UUpdateNextTargetPointBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 *NodeMemory) {
	//Obtenir un pointeur sur AIGuardController
	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	//Appeler la fonction UpdateNextTargetPoint qui contient la logique pour selectionner
	// le prochain TargePoint
	AIGuardController->UpdateNextTargetPoint();

	//Nous retournous Succeeded
	return EBTNodeResult::Succeeded;

}

/** Permet de définir une description pour la tâche. C'est ce texte qui apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
FString UUpdateNextTargetPointBTTaskNode::GetStaticDescription() const {
	return TEXT("Actualisation du point suivant dans le chemin de patrouille");
}


