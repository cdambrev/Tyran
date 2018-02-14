// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponLoot.h"

AWeaponLoot::AWeaponLoot() {

}

void AWeaponLoot::OnUsed(APawn * InstigatorPawn)
{
	ATyranCharacter* MyPawn = Cast<ATyranCharacter>(InstigatorPawn); 
	
	if (MyPawn) {
		/* CHoisir le bon type d'objet à prendre et ajouter à l'inventaire */ 
		if (!MyPawn->WeaponSlotAvailable(WeaponClass->GetDefaultObject<AWeapon>()->GetStorageSlot()))
			MyPawn->DropWeapon();

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnInfo);

		MyPawn->AddWeapon(NewWeapon);
		Destroy();
		{ 
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Emplacement d'arme déjà pris!"); 
		} 
	}
}
