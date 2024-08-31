// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MengOverlay.generated.h"

/**
 * 
 */
UCLASS()
class UNMENG_API UMengOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent) const;
	void SetStaminaBarPercent(float Percent) const;
	void SetGold(int32 Gold) const;
	void SetSoul(int32 Soul) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SoulText;
};
