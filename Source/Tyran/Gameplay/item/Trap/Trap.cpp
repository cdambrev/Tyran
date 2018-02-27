// Fill out your copyright notice in the Description page of Project Settings.

#include "Trap.h"
#include "Basic/TyranCharacter.h"


// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bIsTriggered = false;
	
	trapOwner = EAlignement::A_TYRAN;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap"));
	myMesh->SetupAttachment(RootComponent);
	placementCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementCollision"));
	placementCollision->SetupAttachment(myMesh);
	triggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	triggerZone->SetupAttachment(myMesh);

}

void ATrap::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrap, PawnOwner);
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*> OverlappingActors;
	triggerZone->GetOverlappingActors(OverlappingActors);
	for (auto& i : OverlappingActors) {
		if (ATyranCharacter* character = Cast<ATyranCharacter>(i)) {
			//FString f = EAlignementToString(character->getAlignement());
			//FString nom = character->GetName();
			//UE_LOG(LogTemp, Warning,TEXT("%s Alignement %s"),*nom,*f );
			if (character->getAlignement() != trapOwner && !bIsTriggered)
			{
				trapTarget = character;
				triggered();
			}
		}
	}
}



void ATrap::SetOwningPawn(APawn * NewOwner)
{
	if (PawnOwner != NewOwner) {
		//Instigator = NewOwner;
		PawnOwner = NewOwner;
		// Propriétaire réseau pour les appels RPC. 
		//SetOwner(NewOwner);
	}
	if (ATyranCharacter* character = Cast<ATyranCharacter>(NewOwner))
	{
		if (character->getAlignement() == EAlignement::A_REVOLUTIONNAIRE)
		{
			trapOwner = EAlignement::A_REVOLUTIONNAIRE;
		}
	}
	else {
		trapOwner = EAlignement::A_TYRAN;
	}
}

