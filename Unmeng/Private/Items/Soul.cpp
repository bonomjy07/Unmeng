// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"

#include "Unmeng/Interfaces/PickupInterface.h"

ASoul::ASoul()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);

		SpawnPickupEffect();
		SpawnPickupSound();

		Destroy();
	}
}

// Called every frame
void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

