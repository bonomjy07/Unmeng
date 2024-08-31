// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unmeng/Interfaces/HitInterface.h"
#include "Breakable.generated.h"

UCLASS()
class UNMENG_API ABreakable : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakable();

protected:
	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> Capsule;

	UPROPERTY(EditAnywhere, Category = "Treasure")
	TSubclassOf<class ATreasure> TreasureClass;

	UPROPERTY(EditAnywhere, Category = "Treasure")
	bool bBroken;
};
