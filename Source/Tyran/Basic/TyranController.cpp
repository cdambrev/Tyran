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
	if (captureMap)
		captureMap->update();
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

void ATyranController::updateSelfMap() {
	if (captureMap && revUI) {
		captureMap->update();
		UImage * img = Cast<UImage>(revUI->GetWidgetFromName(TEXT("MiniMap")));
		img->SetBrushFromTexture(captureMap->GetTextureAtLocation(GetPawn()->GetActorLocation()));
	}
	
		
//	UImage * img = Cast<UImage>(revUI->WidgetTree->FindWidget("MiniMap"));
//	img->SetBrushFromMaterial(capture->GetMaterialAtLocation(GetPawn()->GetActorLocation()));
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