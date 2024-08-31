// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MengOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMengOverlay::SetHealthBarPercent(const float Percent) const
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UMengOverlay::SetStaminaBarPercent(const float Percent) const
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UMengOverlay::SetGold(const int32 Gold) const
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void UMengOverlay::SetSoul(int32 Soul) const
{
	if (SoulText)
	{
		const FString String = FString::Printf(TEXT("%d"), Soul);
		const FText Text = FText::FromString(String);
		SoulText->SetText(Text);
	}
}
