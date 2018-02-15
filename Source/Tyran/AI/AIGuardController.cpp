// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGuardController.h"
#include "EngineUtils.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h" 
#include "Runtime/AIModule/Classes/BrainComponent.h" 
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/Character.h"
#include "Basic/TyranCharacter.h"

AAIGuardController::AAIGuardController() {
	patrolPoints.Empty();
}

/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du Behavior Tree pour actualiser le chemin de patrouille */
void AAIGuardController::UpdateNextTargetPoint() {
	// Obtenir la reference au composant blackboard de AIController
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	// Nous r�cup�rons dans targetPointNumber la valeur de la cl� targetPointNumber du blackboard
	// Ce nombre repr�sente l'ordre de parcours du chemin de patrouille
	int32 targetPointNumber = BlackboardComponent->GetValueAsInt("TargetPointNumber");

	// Comme nous n'avons que 2 TargetPoint, quand nous sommes rendu au dernier, on revient au premier
	if (targetPointNumber >= nbTargetPoint)
	{
		// Modifier aussi la valeur de la cl� TargetPointNumber du blackboard
		targetPointNumber = 0;
		BlackboardComponent->SetValueAsInt("TargetPointNumber", targetPointNumber);
	}

	// Pour tous les AIGuardTargetPoint du niveau
	for (AAIGuardTargetPoint* targetPoint : patrolPoints)
	{
		// Si la cl� targetPointNumber du blackboard est �gale � l'attribut Posiiton du point � traiter
		// Ce sera le point suivant dans le chemin du NPC et nous modifierons la cl� TargetPointPosition du blackboard
		// avec la position de cet acteur
		if (targetPointNumber == targetPoint->Position)
		{
			// La cl� targetPointPosition prend la valeur de la position de ce targetPoint du niveau
			// Nous pouvons donc faire "break"
			BlackboardComponent->SetValueAsVector("TargetPointPosition", targetPoint->GetActorLocation());
			break;
		}
	}
	// Finalement, nous incr�mentons la valeur de targetPointNumber (dans le blackboard)
	BlackboardComponent->SetValueAsInt("TargetPointNumber", (targetPointNumber + 1));
}

/** * Nous v�rifions si le personnage est pr�s et si c'est le cas, nous pla�ons 
* une r�f�rence dans le BlackBoard. Cette fonction sera utilis�e par le service 
* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque 
* nous approchons de sa zone de patrouille. */
void AAIGuardController::CheckNearbyEnemy() {

	// Nous obtenons un pointeur sur le pion du NPC
	APawn *pawn = GetPawn();

	// Start: La position du NPC
	FVector multiSphereStart = pawn->GetActorLocation();

	// Nous cr�ons un vecteur � partir de la position du NPC + 15 unit�s en Z. 
	// Ce sont les deux valeurs qui serviront comme points de d�part pour MultiSphereTraceForObjects
	FVector multiSphereEnd = multiSphereStart + FVector(0, 0, 15.0f);

	// Nous cr�ons un tableau que nous utiliserons comme param�tre ObjectTypes dans 
	// MultiSphereTraceForObjects, nous y d�finissons les types d'objets dont il faut tenir compte
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Cr�ation d'un tableau d'acteurs � ignorer, pour l'instant seul le pion du NPC y sera.
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(pawn);

	TArray<FHitResult> OutHits;

	bool result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),// Le monde
		multiSphereStart, // Point de d�part de la ligne qui d�fini la multisph�re
		multiSphereEnd,// Fin de la ligne qui d�fini la multisph�re
		2000,// Rayon de la sph�re
		objectTypes,// Types d'objets dont il faut tenir compte
		false,// false car nous n'utilisons pas le mode complexe
		actorsToIgnore,// Acteurs � ignorer
		EDrawDebugTrace::ForDuration,// Le type de Debug
		OutHits,// O� seront stock�s les r�sultats
		true // true s'il faut ignorer l'objet lui-m�me
	);

	// La cl� TargetActorToFollow est initialis�e � NULL pour le cas o� il n'y aurait 
	// pas d'objet dans la multisph�re.
	UBlackboardComponent* blackboardComponent = BrainComponent->GetBlackboardComponent();
	blackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);

	if (result == true) {
		for (int32 i = 0; i < OutHits.Num(); ++i) {
			FHitResult hit = OutHits[i];

			//AActor* character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			
			// Si l'acteur d�tect� est le joueur ...
			ATyranCharacter *player = dynamic_cast<ATyranCharacter*>(hit.GetActor());
			if (player && player->getAlignement()==EAlignement::A_REVOLUTIONNAIRE) {
				// ... et qu'il est visible, alors modification de la cle TargetActotrToFollow avec un pointeur sur le joueur
				if (player->isVisible) {
					blackboardComponent->SetValueAsObject("TargetActorToFollow", player);
				}
			}
		}
	}
}

EPathFollowingRequestResult::Type AAIGuardController::MoveToEnemy() {

	UBlackboardComponent* blackboardComponent = BrainComponent->GetBlackboardComponent();

	AActor* HeroCharacterActor = Cast<AActor>(blackboardComponent->GetValueAsObject("TargetActorToFollow"));

	// attention, il faut v�rifier qu'il y ait une line of sight.
	EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(HeroCharacterActor, acceptanceRadius);

	return MoveToActorResult;
}

void AAIGuardController::setPatrolPoint(TArray<AAIGuardTargetPoint*> patrolPoints_) {
	if (patrolPoints.Num()) {
		for (AAIGuardTargetPoint* targetPoint : patrolPoints) {
			targetPoint->Destroy();
		}
		patrolPoints.Empty();
	}
	patrolPoints = patrolPoints_;
}

TArray<AAIGuardTargetPoint*> AAIGuardController::getPatrolPoints() {
	return patrolPoints;
}