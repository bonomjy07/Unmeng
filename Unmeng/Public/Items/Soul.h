// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Soul.generated.h"

UCLASS()
class UNMENG_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:	
	ASoul();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls = 10;
	
public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }

};
