// Fill out your copyright notice in the Description page of Project Settings.

#include "Basic/TyranController.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include <Image.h>
#include "GUI/RevHUD.h"
//#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void ATyranController::setTyran(bool b)
{

	isTyran = b;
	if (b) {
		initOnTyranClient();
	}
	else
	{
		
		initOnRevolutionnaireClient();
		
	}
}

void ATyranController::initOnTyranClient_Implementation()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATyranController::initOnRevolutionnaireClient_Implementation()

{
	
}


void ATyranController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATyranController, isTyran);
}

void ATyranController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
}



ATyranController::ATyranController() {
	isTyran = false;

}

void ATyranController::updateMap_Implementation() {
	if(GetHUD())
		Cast<ARevHUD>(GetHUD())->setMapUpdateState(true);
}