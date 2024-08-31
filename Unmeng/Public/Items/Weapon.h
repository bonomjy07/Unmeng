// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class UNMENG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void BeginPlay() override;

	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void PlayEquipSound() const;

	
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	
	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

public:
	TArray<AActor*> IgnoreActors;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
