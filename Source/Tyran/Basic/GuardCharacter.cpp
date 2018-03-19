// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"
#include "Gameplay/TyranOnly/TyranViewSpotLightComponent.h"
#include "Components/CapsuleComponent.h"
#include <ConstructorHelpers.h>


AGuardCharacter::AGuardCharacter() {
	alignement = EAlignement::A_TYRAN;
	//Blueprint'/Game/Blueprints/TyranViewComponent.TyranViewComponent'
/*
	}
	*/
}

void AGuardCharacter::BeginPlay() {
	//static ConstructorHelpers::FObjectFinder<UObject> tyranViewFindHelper(TEXT("/Game/Blueprints/TyranViewComponent"));
	//if (tyranViewFindHelper.Succeeded()) {
	//	USpotLightComponent* tyranView = static_cast<USpotLightComponent*>(tyranViewFindHelper.Object);
	//	tyranView->SetupAttachment(GetCapsuleComponent());
	//}

}

void AGuardCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGuardCharacter, modeGuard);
	DOREPLIFETIME(AGuardCharacter, fuiteAutorise);
}
