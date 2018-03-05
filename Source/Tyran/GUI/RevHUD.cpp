// Fill out your copyright notice in the Description page of Project Settings.

//#include "RevHUD.h"
//#include <ConstructorHelpers.h>
//#include <Image.h>
//
//
//ARevHUD::ARevHUD() {
//	ConstructorHelpers::FClassFinder<UUserWidget> defaultUIHelper(TEXT("/Game/UI/RevolutionnaireInterface"));
//	defaultUIClass = defaultUIHelper.Class;
//	
//	//static ConstructorHelpers::FClassFinder<ACaptureMiniMap> captureMapClass(TEXT("/Game/UI/MiniMapCapture"));
//	//if (captureMapClass.Class != NULL) {
//	//	defaultCapture = captureMapClass.Class;
//	//}
//}
//
//void ARevHUD::BeginPlay() {
//	Super::BeginPlay();
//	static_cast<APlayerController*>(GetOwningPlayerController())->SetInputMode(FInputModeGameAndUI());
//	
//	// default UI
//	defaultUIWidget = CreateWidget<UUserWidget>(static_cast<APlayerController*>(GetOwningPlayerController()), defaultUIClass);
//	defaultUIWidget->AddToViewport(9998);
//	
//	//captureMap = GetWorld()->SpawnActor<ACaptureMiniMap>(defaultCapture);
//	//if (captureMap)
//	//	captureMap->update();
//}
//
//void ARevHUD::displayDefaultUI() {
//	defaultUIWidget->SetVisibility(ESlateVisibility::Visible);
//}
//
//void ARevHUD::removeDefaultUI() {
//	defaultUIWidget->SetVisibility(ESlateVisibility::Hidden);
//}