// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemyBTService.h"
#include "AIGuardController.h"

UCheckEnemyBTService::UCheckEnemyBTService() {
	NodeName = "CheckNearbyEnemy";

	Interval = 0.5f;

	RandomDeviation = 0.1f;
}

// Sera appelee a chaque update du service
void UCheckEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIGuardController *AIGuardController = Cast<AAIGuardController>(OwnerComp.GetOwner());

	// Appeler la fonction CheckNearbyEnemy du AIEnemyController dans lequel nous avions 
	// programmé la logique pour déterminer si le personnage était autour et au besoin 
	// pour modifier la clé du blackboard en conséquence
	AIGuardController->CheckNearbyEnemy();
}