// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "MengCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
	StartLocation = GetActorLocation();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EItemState::EIS_Hovering)
	{
		Hover(DeltaTime);
	}
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Pickup = Cast<IPickupInterface>(OtherActor);
	if (Pickup)
	{
		Pickup->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* Pickup = Cast<IPickupInterface>(OtherActor);
	if (Pickup)
	{
		Pickup->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupEffect()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,PickupEffect,GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

void AItem::Hover(float DeltaTime)
{
	// Floating
	FVector NewLocation = StartLocation;
	const float DeltaHeight = FMath::Sin(GetWorld()->GetTimeSeconds() * Frequency) * Amplitude;
	NewLocation.Z += DeltaHeight;
	SetActorLocation(NewLocation);

	// Rotating
	FRotator NewRotation = GetActorRotation();
	NewRotation += RotationSpeed * DeltaTime;
	SetActorRotation(NewRotation);
}
