// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/Breakable.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakable::ABreakable()
{
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);	
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakable::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// Check Blueprint
	
	return;
	/*
	UWorld* World = GetWorld();
	if (World && TreasureClass)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(TreasureClass, Location, GetActorRotation());
	}
	*/
}
