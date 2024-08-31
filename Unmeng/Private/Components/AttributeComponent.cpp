// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Health Functions
void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

// Stamina Functions
void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

// Resource Management Functions
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}
