// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MengCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class UNMENG_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

	virtual void Die() override;
	bool IsDead();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	void SpawnSoul();

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHealthBarV3Component> HealthWidgetCompo;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensing;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	void CheckPatrolTarget();
	void CheckCombatTarget();
	void ClearPatrolTimer();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<class AAIController> EnemyController;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	TArray<AActor*> PatrolTargetList;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;
	
	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	bool InTargetRange(const AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

private:
	/**
	 *  AI Behavior
	 */
	void InitializeEnemy();
	void HideHealthBar() const;
	void ShowHealthBar() const;
	void LooseInterest();
	void StartPatrolling();
	bool IsOutsideCombatRadius();
	void ChaseTarget();
	bool IsOutsideAttackRadius();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	void ClearAttackTimer();
	void SpawnDefaultWeapon();

	/**
	 * Combat
	 */
	void StartAttackTimer();
	bool IsEngaged();
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.0f;
};
