// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Characters/CharacterTypes.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarV3Component.h"
#include "Items/Soul.h"
#include "Items/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	/*
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	*/
	
	HealthWidgetCompo = CreateDefaultSubobject<UHealthBarV3Component>(TEXT("HealthBarV3Component"));
	HealthWidgetCompo->SetupAttachment(GetRootComponent());

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	InitializeEnemy();
	Tags.Add(FName("Enemy"));
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("hand_r_Socket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::Die()
{
	Super::Die();
	
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnSoul();
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
		return;
	}

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	
	if (!IsDead())
	{
		ShowHealthBar();
	}
	
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}
	
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		FVector Location = GetActorLocation();
		FVector Backward = GetActorForwardVector() * -1.f * 30.f;
		FVector SpawnLocation = Location + Backward;
		
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSoul());
		}
	}
}

void AEnemy::HideHealthBar() const
{
	if (HealthWidgetCompo)
	{
		HealthWidgetCompo->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar() const
{
	if (HealthWidgetCompo)
	{
		HealthWidgetCompo->SetVisibility(true);
	}
}

void AEnemy::LooseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 125.f;
	MoveToTarget(PatrolTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearAttackTimer()
{
	// UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %hhd"), EnemyState);
	UE_LOG(LogTemp, Warning, TEXT("Clear Attack Timer"));

	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LooseInterest();
		if (!IsEngaged())
		{
			StartPatrolling();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			ChaseTarget();
		}	
		
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (CanAttack())
	{
		// ClearAttackTimer();
		StartAttackTimer();
		
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));
	
	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr)
	{
		return;
	}

	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	const bool bCanAttack =
		IsInsideAttackRadius()
		&& !IsAttacking()
		&& !IsEngaged()
		&& !IsDead(); 
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	
	if (HealthWidgetCompo)
	{
		HealthWidgetCompo->SetHealthPercent(Attributes->GetHealthPercent());
	}	
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

bool AEnemy::InTargetRange(const AActor* Target, const double Radius)
{
	if (Target == nullptr)
	{
		return false;
	}
	
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargetList)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}
