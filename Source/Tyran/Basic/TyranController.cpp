// Fill out your copyright notice in the Description page of Project Settings.

#include "Basic/TyranController.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/LightComponent.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include <Image.h>
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
	revUI = CreateWidget<UUserWidget>(GetGameInstance(), revolutionnaireUIClass);
	revUI->AddToViewport(9999);
	captureMap = GetWorld()->SpawnActor<ACaptureMiniMap>(defaultCapture);
	updateMap();
}

void ATyranController::Tick(float DeltaSeconds)
{
	if (!isTyran && revUI && captureMap) {
		if (updateMapNextTick) {
			updateMap();
			setMapUpdateState(false);
		}

		moveMiniMap();
	}
	
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

void ATyranController::updateMap_Implementation() {
	if (captureMap && revUI) {
		captureMap->update();
	}
	
}

void ATyranController::setMapUpdateState_Implementation(bool updateNextTick) {
	updateMapNextTick = updateNextTick;
}

void ATyranController::moveMiniMap() {
	if (captureMap && revUI) {
		UImage * img = Cast<UImage>(revUI->GetWidgetFromName(TEXT("MiniMap")));
		if (GetPawn()) {
			img->SetBrushFromTexture(captureMap->GetTextureAtLocation(GetPawn()->GetActorLocation()));
		}
			
	}

}

ATyranController::ATyranController() {
	isTyran = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> revUIHelper(TEXT("/Game/UI/RevolutionnaireInterface"));
	revolutionnaireUIClass = revUIHelper.Class;

	static ConstructorHelpers::FClassFinder<ACaptureMiniMap> captureMapClass(TEXT("/Game/UI/MiniMapCapture"));
	if (captureMapClass.Class != NULL)
	{
		defaultCapture = captureMapClass.Class;
	}

}