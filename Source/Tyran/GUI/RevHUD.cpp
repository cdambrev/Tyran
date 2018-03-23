// Fill out your copyright notice in the Description page of Project Settings.

#include "RevHUD.h"
#include <ConstructorHelpers.h>
#include <Image.h>
#include "Tools/Debug/DebugTools.h"
#include "Basic/TyranCharacter.h"

ARevHUD::ARevHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> defaultUIHelper(TEXT("/Game/UI/RevolutionnaireInterface"));
	defaultUIClass = defaultUIHelper.Class;
	
	static ConstructorHelpers::FClassFinder<ACaptureMiniMap> captureMapClass(TEXT("/Game/UI/MiniMapCapture"));
	if (captureMapClass.Class != NULL) {
		defaultCapture = captureMapClass.Class;
	}
	updateMapNextTick = true;
	
	ConstructorHelpers::FClassFinder<UUserWidget> inventoryFullUIHelper(TEXT("/Game/UI/InventoryFull"));
	inventoryFullUIClass = inventoryFullUIHelper.Class;
}

void ARevHUD::BeginPlay() {
	Super::BeginPlay();
	//static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
	
	// default UI
	defaultUIWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), defaultUIClass);
	defaultUIWidget->AddToViewport(9998);
	
	captureMap = GetWorld()->SpawnActor<ACaptureMiniMap>(defaultCapture);
	if (captureMap)
		captureMap->update();
}

void ARevHUD::Tick(float DeltaSeconds) {
	if (updateMapNextTick) {
		updateMap();
		setMapUpdateState(false);
	}
	
	moveMiniMap();
}

void ARevHUD::updateMap() {
	captureMap->update();
}

void ARevHUD::moveMiniMap() {
	UImage * img = Cast<UImage>(defaultUIWidget->GetWidgetFromName(TEXT("MiniMap")));
#ifdef DEBUG_ON
	Debugger::get().addTextLog("Move Mini Map of " + GetOwningPlayerController()->GetDebugName(GetOwningPawn()), "hud");
#endif
	if(GetOwningPawn())
		img->SetBrushFromTexture(captureMap->GetTextureAtLocation(GetOwningPawn()->GetActorLocation()));
}

void ARevHUD::setMapUpdateState(bool updateNextTick) {
	updateMapNextTick = updateNextTick;
}

void ARevHUD::displayDefaultUI() {
	defaultUIWidget->SetVisibility(ESlateVisibility::Visible);
}

void ARevHUD::removeDefaultUI() {
	defaultUIWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ARevHUD::drawInventoryFull() {
	CreateWidget<UUserWidget>(GetOwningPlayerController(), inventoryFullUIClass);
}