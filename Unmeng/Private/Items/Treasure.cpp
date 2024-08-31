// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"

#include "MengCharacter.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Pickup = Cast<IPickupInterface>(OtherActor);
	if (Pickup)
	{
		Pickup->AddGold(this);

		SpawnPickupSound();
		Destroy();
	}
}
