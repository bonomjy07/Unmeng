#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Unmeng/Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class UNMENG_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void Attack();
	virtual bool CanAttack();
	virtual void Die();
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override; // from IHitinterface

	virtual int PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName) const;
	void StopAttackMontage();
	virtual void PlayDodgeMontage();
	virtual int32 PlayDeathMontage();
	
	// On Hit
	void DirectionalHitReact(const FVector& ImpactPoint) const;
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName) const;
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	TObjectPtr<AActor> CombatTarget;

	TObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	// Attack AM
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;
	
	// Death AM
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
	
	// On Hit AM
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;

	// Dodge AM
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAttributeComponent> Attributes;

	// Hit
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UParticleSystem> HitParticles;

	// TODO Motion Warping
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;
	
protected:
	bool IsAlive();

	void DisableCapsule();
	void DisableMeshCollision();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled) const;
	
public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
};
