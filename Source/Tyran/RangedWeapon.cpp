// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedWeapon.h"
#include "TyranCharacter.h"
#include "Tyran.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ARangedWeapon::ARangedWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshWeapon = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponMesh"));
	MeshWeapon->bReceivesDecals = false;
	MeshWeapon->CastShadow = true;
	MeshWeapon->SetCollisionObjectType(ECC_WorldDynamic);
	MeshWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshWeapon->SetCollisionResponseToAllChannels(ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;

}

ATyranCharacter * ARangedWeapon::GetPawnOwner() const
{
	return Pawn;
}

void ARangedWeapon::AttachMeshToPawn()
{
	if (Pawn)
	{
		// Remove and hide both first and third person meshes
		DetachMeshFromPawn();

		// For locally controller players we attach both weapons and let the bOnlyOwnerSee, bOwnerNoSee flags deal with visibility.
		FName AttachPoint = Pawn->GetWeaponAttachPoint();
		//if (MyPawn->IsLocallyControlled() == true)
		{
			USkeletalMeshComponent* PawnMesh = Pawn->GetMesh();
			MeshWeapon->SetHiddenInGame(false);
			MeshWeapon->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
		/*else
		{
			USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
			USkeletalMeshComponent* UsePawnMesh = MyPawn->GetPawnMesh();
			UseWeaponMesh->AttachToComponent(UsePawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			UseWeaponMesh->SetHiddenInGame(false);
		}*/
	}
}

void ARangedWeapon::DetachMeshFromPawn()
{
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

