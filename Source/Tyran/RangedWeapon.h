// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.generated.h"

class ATyranCharacter;
//class UStaticMeshComponent;

UCLASS()
class TYRAN_API ARangedWeapon : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	//ARangedWeapon();

	/** set the weapon's owning pawn */
	void SetOwningPawn(ATyranCharacter* AShooterCharacter);

	/** get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	class ATyranCharacter* GetPawnOwner() const;

protected:
	//////////////////////////////////////////////////////////////////////////
	// Inventory
	/** attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(Transient)
	class ATyranCharacter* Pawn;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* MeshWeapon;

};
