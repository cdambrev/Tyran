// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Sound/SoundCue.h" 
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "ImpactEffect.generated.h"

UCLASS(ABSTRACT, Blueprintable)
class TYRAN_API AImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	/* FX implanté sur matériaux standards */
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* DefaultFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* FleshFX;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* DefaultSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* FleshSound;

	FHitResult SurfaceHit;

	// Sets default values for this actor's properties
	AImpactEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UParticleSystem* GetImpactFX(EPhysicalSurface SurfaceType) const; 
	USoundCue* GetImpactSound(EPhysicalSurface SurfaceType) const;

	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
