// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/TyranOnly/Placeable_Object/PlaceableObject.h"
#include "Basic/Enum/Alignement.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Trap.generated.h"


UCLASS(abstract)
class TYRAN_API ATrap : public APlaceableObject
{
	GENERATED_BODY()
	
protected:
	// Sets default values for this actor's properties
	ATrap();
public:

	EAlignement trapOwner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* myMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* placementCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* triggerZone;

	

	bool isTriggered;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void triggered() { check(0 && "You must override this"); }

	
	virtual void SetOwningPawn(APawn * NewOwner) ;
	
	
};
