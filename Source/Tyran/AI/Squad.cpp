// Fill out your copyright notice in the Description page of Project Settings.

#include "Squad.h"
#include "Basic/TyranGameMode.h"
#include "Tools/Debug/DebugTools.h"

// Sets default values
ASquad::ASquad()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASquad::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld()->GetAuthGameMode()){
		Cast<ATyranGameMode>(GetWorld()->GetAuthGameMode())->squads.Add(this);
#ifdef DEBUG_ON
		Debugger::get().addTextLog("[CONSTRUCTION] Squad", "ia");
#endif
	}
		
}

// Called every frame
void ASquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ASquad::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASquad::addGarde(AAIGuardController & garde) {
	gardes.Add(&garde);
	FString s = "";
	for (AAIGuardController * g : gardes) {
		s += g->GetName();
		s += " | ";
	}
#ifdef DEBUG_ON
	Debugger::get().addTextLog("[AJOUT] Squad compose de : " + s, "ia");
#endif
}

void ASquad::removeGarde(AAIGuardController & garde) {
#ifdef DEBUG_ON
	FString s = "";
	for (AAIGuardController * g : gardes) {
		s += g->GetName();
		s += " | ";
	}
	Debugger::get().addTextLog("[RETRAIT] Squad compose de : " + s, "ia");
#endif
	gardes.Remove(&garde);

	if (empty()) {
		Cast<ATyranGameMode>(GetWorld()->GetAuthGameMode())->squads.Remove(this);
		Destroy();
#ifdef DEBUG_ON
		Debugger::get().addTextLog("[DESTRUCTION] Squad", "ia");
#endif
	}


}

bool ASquad::empty() {
	return gardes.Num() == 0;
}

TArray<AAIGuardController*> & ASquad::getGardes()
{
	return gardes;
}



