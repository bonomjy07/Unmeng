// Fill out your copyright notice in the Description page of Project Settings.


#include "MengCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/MengHUD.h"
#include "HUD/MengOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapon.h"

// Sets default values
AMengCharacter::AMengCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Not sure
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void AMengCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));

	InitializeOverlay();
}

void AMengCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Attributes && Overlay)
	{
		Attributes->RegenStamina(DeltaSeconds);
		Overlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AMengCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMengCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMengCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMengCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMengCharacter::LookUp);

	// Action
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AMengCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMengCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AMengCharacter::Dodge);
}

void AMengCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void AMengCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

float AMengCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetOverlayHealth();
	return DamageAmount;
}

void AMengCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AMengCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && Overlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		Overlay->SetSoul(Attributes->GetSoul());
	}
}

void AMengCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && Overlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		Overlay->SetGold(Attributes->GetGold());
	}
}

void AMengCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}
	
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMengCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)
	{
		return;
	}
	
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMengCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMengCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMengCharacter::EKeyPressed()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}	
}

void AMengCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("hand_r_Socket"), this, this);
	CharacterState = ECharacterState::ECS_Equipped;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AMengCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina())
	{
		return;
	}
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && Overlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		Overlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AMengCharacter::Attack()
{
	Super::Attack();
	
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AMengCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void AMengCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMengCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

bool AMengCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMengCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}

void AMengCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMengCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_Equipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMengCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_05_Socket"));
	}
}

void AMengCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("hand_r_Socket"));
	}
}

void AMengCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMengCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMengCharacter::Die()
{
	Super::Die();
	
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool AMengCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool AMengCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void AMengCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AMengCharacter::InitializeOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AMengHUD* MengHUD = Cast<AMengHUD>(PlayerController->GetHUD());
		if (MengHUD)
		{
			 Overlay = MengHUD->GetOverlay();
			if (Overlay && Attributes)
			{
				Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				Overlay->SetStaminaBarPercent(1.f);
				Overlay->SetGold(Attributes->GetGold());
				Overlay->SetSoul(Attributes->GetSoul());
			}	
		}
	}
}

bool AMengCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AMengCharacter::SetOverlayHealth()
{
	if (Overlay && Attributes)
	{
		Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

