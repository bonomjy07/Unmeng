// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarV3Component.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"

void UHealthBarV3Component::SetHealthPercent(float Percent)
{
	if (BarWidget == nullptr)
	{
		BarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (BarWidget && BarWidget->HealthBar)
	{
		BarWidget->HealthBar->SetPercent(Percent);
	}
}
