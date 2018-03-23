// Fill out your copyright notice in the Description page of Project Settings.

#include "RevHUD.h"
#include <ConstructorHelpers.h>
#include <Image.h>
#include "Tools/Debug/DebugTools.h"
#include "Basic/TyranCharacter.h"
#include "Basic/TyranController.h"
#include "Basic/TyranGameState.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

ARevHUD::ARevHUD() {
	ConstructorHelpers::FClassFinder<UUserWidget> defaultUIHelper(TEXT("/Game/UI/RevolutionnaireInterface"));
	defaultUIClass = defaultUIHelper.Class;
	
	static ConstructorHelpers::FClassFinder<ACaptureMiniMap> captureMapClass(TEXT("/Game/UI/MiniMapCapture"));
	if (captureMapClass.Class != NULL) {
		defaultCapture = captureMapClass.Class;
	}
	updateMapNextTick = true;
}

void ARevHUD::BeginPlay() {
	Super::BeginPlay();
	//static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
	
	// default UI
	defaultUIWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), defaultUIClass);
	defaultUIWidget->AddToViewport(9998);
	
	captureMap = GetWorld()->SpawnActor<ACaptureMiniMap>(defaultCapture);
	if (captureMap) {
		updateMap();
	}
		

	lastPos = GetOwningPawn()->GetActorLocation();
	
}

void ARevHUD::Tick(float DeltaSeconds) {
	if (updateMapNextTick) {
		updateMap();
		setMapUpdateState(false);
	}

	if (GetOwningPawn()) {
		
		FVector currentPos = GetOwningPawn()->GetActorLocation();
		if (currentPos != lastPos) {
			moveMiniMap();
			lastPos = currentPos;
		}
	}

	
}

void ARevHUD::updateMap() {
	captureMap->update();
	moveMiniMap();
}

void ARevHUD::moveMiniMap() {

	for (ATyranCharacter *  c : GetWorld()->GetGameState<ATyranGameState>()->playersPawn) {
		auto v1 = c;
		auto v2 = v1;
	}
	UImage * img = Cast<UImage>(defaultUIWidget->GetWidgetFromName(TEXT("MiniMap")));
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