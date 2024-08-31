// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class UNMENG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	// Overlapping
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Effects
	virtual void SpawnPickupEffect();
	virtual void SpawnPickupSound();

	// Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;

	// Effects
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> ItemEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> PickupEffect;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> PickupSound;
	
	// Floating
	UPROPERTY(EditAnywhere, Category = "Floating")
	float Amplitude = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Floating")
	float Frequency = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Floating")
	FRotator RotationSpeed = FRotator(0.0f, 45.0f, 0.0f);

	EItemState State = EItemState::EIS_Hovering;

private:
	void Hover(float DeltaTime);
	
	FVector StartLocation;
};
