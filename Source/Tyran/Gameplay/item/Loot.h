// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot.generated.h"

UCLASS()
class TYRAN_API ALoot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxFrequency;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh") 
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULootInteractionComponent* lootInteraction;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Appelé quand le joueur interagit avec l'objet */ 
	virtual void OnUsed(APawn* InstigatorPawn){}

	/* Accesseur public au composant! */ 
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { 
		return MeshComp; 
	}

	
};
