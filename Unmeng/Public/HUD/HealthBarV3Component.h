// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarV3Component.generated.h"

/**
 * 
 */
UCLASS()
class UNMENG_API UHealthBarV3Component : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);
protected:
	
	UPROPERTY()
	TObjectPtr<class UHealthBar> BarWidget;
};
