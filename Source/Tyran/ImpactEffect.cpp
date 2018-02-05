// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpactEffect.h"

#define SURFACE_DEFAULT SurfaceType_Default
#define SURFACE_FLESH SurfaceType1

// Sets default values
AImpactEffect::AImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bAutoDestroyWhenFinished = true;
}

// Called when the game starts or when spawned
void AImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

UParticleSystem * AImpactEffect::GetImpactFX(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType) { 
	case SURFACE_DEFAULT:
		return DefaultFX; 
	case SURFACE_FLESH: 
		return FleshFX;
	default: 
		return nullptr; 
	}
}

USoundCue * AImpactEffect::GetImpactSound(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType) { 
	case SURFACE_DEFAULT:
		return DefaultSound; 
	case SURFACE_FLESH: 
		return FleshSound; 
	default: 
		return nullptr; 
	}
}

void AImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 
	
	/* Figure out what we hit (SurfaceHit is setting during actor instantiation in weapon class) */
	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get(); 
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat); 
	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType); 
	
	if (ImpactFX) { 
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation()); 
	} 
	
	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType); 
	if (ImpactSound) { 
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation()); 
	}
}

// Called every frame
void AImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

