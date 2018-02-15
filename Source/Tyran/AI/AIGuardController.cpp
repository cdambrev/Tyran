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

/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du Behavior Tree pour actualiser le chemin de patrouille */
void AAIGuardController::UpdateNextTargetPoint() {
	// Obtenir la reference au composant blackboard de AIController
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	// Nous récupérons dans targetPointNumber la valeur de la clé targetPointNumber du blackboard
	// Ce nombre représente l'ordre de parcours du chemin de patrouille
	int32 targetPointNumber = BlackboardComponent->GetValueAsInt("TargetPointNumber");

	// Comme nous n'avons que 2 TargetPoint, quand nous sommes rendu au dernier, on revient au premier
	if (targetPointNumber >= nbTargetPoint)
	{
		// Modifier aussi la valeur de la clé TargetPointNumber du blackboard
		targetPointNumber = 0;
		BlackboardComponent->SetValueAsInt("TargetPointNumber", targetPointNumber);
	}

	// Pour tous les AIGuardTargetPoint du niveau
	for (AAIGuardTargetPoint* targetPoint : patrolPoints)
	{
		// Si la clé targetPointNumber du blackboard est égale à l'attribut Posiiton du point à traiter
		// Ce sera le point suivant dans le chemin du NPC et nous modifierons la clé TargetPointPosition du blackboard
		// avec la position de cet acteur
		if (targetPointNumber == targetPoint->Position)
		{
			// La clé targetPointPosition prend la valeur de la position de ce targetPoint du niveau
			// Nous pouvons donc faire "break"
			BlackboardComponent->SetValueAsVector("TargetPointPosition", targetPoint->GetActorLocation());
			break;
		}
	}
	// Finalement, nous incrémentons la valeur de targetPointNumber (dans le blackboard)
	BlackboardComponent->SetValueAsInt("TargetPointNumber", (targetPointNumber + 1));
}

/** * Nous vérifions si le personnage est près et si c'est le cas, nous plaçons 
* une référence dans le BlackBoard. Cette fonction sera utilisée par le service 
* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque 
* nous approchons de sa zone de patrouille. */
void AAIGuardController::CheckNearbyEnemy() {

	// Nous obtenons un pointeur sur le pion du NPC
	APawn *pawn = GetPawn();

	// Start: La position du NPC
	FVector multiSphereStart = pawn->GetActorLocation();

	// Nous créons un vecteur à partir de la position du NPC + 15 unités en Z. 
	// Ce sont les deux valeurs qui serviront comme points de départ pour MultiSphereTraceForObjects
	FVector multiSphereEnd = multiSphereStart + FVector(0, 0, 15.0f);

	// Nous créons un tableau que nous utiliserons comme paramètre ObjectTypes dans 
	// MultiSphereTraceForObjects, nous y définissons les types d'objets dont il faut tenir compte
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Création d'un tableau d'acteurs à ignorer, pour l'instant seul le pion du NPC y sera.
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(pawn);

	TArray<FHitResult> OutHits;

	bool result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),// Le monde
		multiSphereStart, // Point de départ de la ligne qui défini la multisphère
		multiSphereEnd,// Fin de la ligne qui défini la multisphère
		2000,// Rayon de la sphère
		objectTypes,// Types d'objets dont il faut tenir compte
		false,// false car nous n'utilisons pas le mode complexe
		actorsToIgnore,// Acteurs à ignorer
		EDrawDebugTrace::ForDuration,// Le type de Debug
		OutHits,// Où seront stockés les résultats
		true // true s'il faut ignorer l'objet lui-même
	);

	// La clé TargetActorToFollow est initialisée à NULL pour le cas où il n'y aurait 
	// pas d'objet dans la multisphère.
	UBlackboardComponent* blackboardComponent = BrainComponent->GetBlackboardComponent();
	blackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);

	if (result == true) {
		for (int32 i = 0; i < OutHits.Num(); ++i) {
			FHitResult hit = OutHits[i];

			//AActor* character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			
			// Si l'acteur détecté est le joueur ...
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

	// attention, il faut vérifier qu'il y ait une line of sight.
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