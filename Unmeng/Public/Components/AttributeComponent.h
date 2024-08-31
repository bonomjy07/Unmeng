// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNMENG_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

public:	
	void ReceiveDamage(float Damage);
	
	float GetHealthPercent();
	
	float GetStaminaPercent();
	
	void UseStamina(float StaminaCost);
	
	void RegenStamina(float DeltaTime);

	bool IsAlive();
	
private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Resource")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Resource")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float DodgeCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 10.f;

public:
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSoul() const { return Souls; }
	
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
};
