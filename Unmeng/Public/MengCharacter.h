// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Unmeng/Interfaces/PickupInterface.h"
#include "MengCharacter.generated.h"

UCLASS()
class UNMENG_API AMengCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AMengCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Jump() override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	
	virtual void Attack() override;
	virtual bool CanAttack() override;
	void EKeyPressed();

	void EquipWeapon(AWeapon* Weapon);

	void Dodge();
	
	// UFUNCTION(BlueprintCallable)
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();
	void Arm();
	void Disarm();
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	UPROPERTY()
	TObjectPtr<class UMengOverlay> Overlay;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> ViewCamera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AItem> OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	void InitializeOverlay();
	
	bool IsUnoccupied();
	bool IsOccupied();
	bool HasEnoughStamina();
	
	void SetOverlayHealth();
};

