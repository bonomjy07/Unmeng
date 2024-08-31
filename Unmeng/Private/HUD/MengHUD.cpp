// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MengHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/MengOverlay.h"


void AMengHUD::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* Controller = World->GetFirstPlayerController();
	if (!Controller)
	{
		return;
	}
	
	if (OverlayClass)
	{
		Overlay = CreateWidget<UMengOverlay>(Controller, OverlayClass);
		Overlay->AddToViewport();
	}
}
