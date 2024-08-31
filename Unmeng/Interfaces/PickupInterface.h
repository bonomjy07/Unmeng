// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNMENG_API IPickupInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(class AItem* Item);
	virtual void AddSouls(class ASoul* Soul);
	virtual void AddGold(class ATreasure* Treasure);

};

inline void IPickupInterface::SetOverlappingItem(AItem* Item)
{
}

inline void IPickupInterface::AddSouls(ASoul* Soul)
{
}

inline void IPickupInterface::AddGold(ATreasure* Treasure)
{
}
