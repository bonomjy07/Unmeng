// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MengHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNMENG_API AMengHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	FORCEINLINE class UMengOverlay* GetOverlay() const { return Overlay; }
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMengOverlay> OverlayClass;

	UPROPERTY()
	TObjectPtr<class UMengOverlay> Overlay;

};
