// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/RangeCombatComponentRH.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacters/ProjectileBaseRH.h"
#include "PlayerCharacters/Components/DodgeComponentRH.h"
#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "PlayerCharacters/Interfaces/CombatRH.h"

// Sets default values for this component's properties
URangeCombatComponentRH::URangeCombatComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URangeCombatComponentRH::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
}


// Called every frame
void URangeCombatComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Continuously rotate towards enemy while charging
	if (bIsCharging)
	{
		RotateTowardsNearestEnemy();
	}
}

void URangeCombatComponentRH::PerformPrimaryRangedAttack()
{
	// Check Cooldown
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastShotTime < ShotCooldown)
		return;

	// Check if dodging
	if (UDodgeComponentRH* DodgeComp = GetOwner()->FindComponentByClass<UDodgeComponentRH>())
	{
		if (DodgeComp->bIsDodging)
		{
			return;
		}
	}

	RotateTowardsNearestEnemy();

	if (!ProjectileClass || !CharacterRef) { return; }

	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		// Disable movement temporarily
		Character->GetCharacterMovement()->DisableMovement();
	}

	if (PrimaryAttackMontage && CharacterRef)
	{
		float MontageDuration = CharacterRef->PlayAnimMontage(PrimaryAttackMontage);
		float SpawnDelay = FMath::Min(MontageDuration, 0.2f); 
		GetWorld()->GetTimerManager().SetTimer(
			PrimaryFireSpawnTimerHandle, this,
			&URangeCombatComponentRH::SpawnPrimaryProjectile,
			SpawnDelay, false);
	}
	else
	{
		// Fallback: spawn immediately if no montage assigned
		SpawnPrimaryProjectile();
	}

	GetWorld()->GetTimerManager().SetTimer(
		PrimaryFireMovementTimerHandle, this,
		&URangeCombatComponentRH::ReEnableMovementAfterPrimaryFire,
		PrimaryFireMovementLockDuration, false);

	LastShotTime = CurrentTime;
}

void URangeCombatComponentRH::StartChargeShot()
{
	// Check cooldown
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastChargeShotTime < ChargeShotCooldown)
	{
		return;
	}

	if (!CharacterRef || bIsCharging) { return; }

	// Check if dodging
	if (UDodgeComponentRH* DodgeComp = GetOwner()->FindComponentByClass<UDodgeComponentRH>())
	{
		if (DodgeComp->bIsDodging)
		{
			return;
		}
	}

	// Disable character movement
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		Character->GetCharacterMovement()->DisableMovement();
	}

	bIsCharging = true;
	
	if (ChargeStartMontage && CharacterRef)
	{
		CharacterRef->PlayAnimMontage(ChargeStartMontage);
	}

	// Start timer to fire charge shot
	GetWorld()->GetTimerManager().SetTimer(
		ChargeShotTimerHandle, this, &URangeCombatComponentRH::FireChargeShot,
		ChargeShotDuration, false);

	UE_LOG(LogTemp, Display, TEXT("Charging shot"));
}

void URangeCombatComponentRH::CancelChargeShot()
{
	if (!bIsCharging) { return; }

	// Re-enable character movement and stop any charge montages
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		if (ChargeStartMontage)
		{
			Character->StopAnimMontage(ChargeStartMontage);
		}
		if (ChargeAttackMontage)
		{
			Character->StopAnimMontage(ChargeAttackMontage);
		}
	}

	// Clear all timers related to the charge shot
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargeShotTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ChargeShotSpawnTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ChargeShotMovementTimerHandle);
	}

	bIsCharging = false;
	UE_LOG(LogTemp, Display, TEXT("Charge shot cancelled"));
}

void URangeCombatComponentRH::FireChargeShot()
{
	if (!ProjectileClass || !CharacterRef) return;

	// Ensure any start montage is stopped
	if (ChargeStartMontage && CharacterRef)
	{
		CharacterRef->StopAnimMontage(ChargeStartMontage);
	}

	// Lock movement while the attack montage / spawn sequence runs
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		Character->GetCharacterMovement()->DisableMovement();
	}

	if (ChargeAttackMontage && CharacterRef)
	{
		float MontageDuration = CharacterRef->PlayAnimMontage(ChargeAttackMontage);
		float SpawnDelay = FMath::Min(MontageDuration, 0.2f); // sync spawn
		float LockDuration = (MontageDuration > 0.0f) ? MontageDuration : ChargeAttackMovementLockDuration;
		
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				ChargeShotMovementTimerHandle, this,
				&URangeCombatComponentRH::ReEnableMovementAfterChargeShot,
				LockDuration, false);
		}

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				ChargeShotSpawnTimerHandle, this,
				&URangeCombatComponentRH::SpawnChargeProjectile,
				SpawnDelay, false);
		}
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				ChargeShotMovementTimerHandle, this,
				&URangeCombatComponentRH::ReEnableMovementAfterChargeShot,
				ChargeAttackMovementLockDuration, false);
		}
		SpawnChargeProjectile();
	}
}

void URangeCombatComponentRH::SpawnPrimaryProjectile()
{
	if (!ProjectileClass || !CharacterRef) { return; }

	FVector SpawnLocation = CharacterRef->GetActorLocation() +
		CharacterRef->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = CharacterRef->GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = CharacterRef->GetInstigator();

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (AProjectileBaseRH* Projectile = Cast<AProjectileBaseRH>(SpawnedProjectile))
	{
		Projectile->DamageType = EDamageTypesRH::GunShot;

		if (ICombatRH* CombatInterface = Cast<ICombatRH>(CharacterRef))
		{
			Projectile->Damage = ICombatRH::Execute_GetDamage(CharacterRef, EDamageTypesRH::GunShot);
		}
	}
}

void URangeCombatComponentRH::SpawnChargeProjectile()
{
	if (!ProjectileClass || !CharacterRef) return;

	FVector SpawnLocation = CharacterRef->GetActorLocation() +
		CharacterRef->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = CharacterRef->GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = CharacterRef->GetInstigator();

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (AProjectileBaseRH* Projectile = Cast<AProjectileBaseRH>(SpawnedProjectile))
	{
		Projectile->DamageType = EDamageTypesRH::ChargeShot;
		if (ICombatRH* CombatInterface = Cast<ICombatRH>(CharacterRef))
		{
			Projectile->Damage = ICombatRH::Execute_GetDamage(CharacterRef, EDamageTypesRH::ChargeShot);
		}
		Projectile->SetActorScale3D(FVector(1.5f));
	}

	bIsCharging = false;
	LastChargeShotTime = GetWorld()->GetTimeSeconds();
}

void URangeCombatComponentRH::RotateTowardsNearestEnemy()
{
	// Find and lock onto nearest enemy when attacking
	if (auto LockOnComp = CharacterRef->FindComponentByClass<ULockOnComponentRH>())
	{
		TArray<AActor*> NearbyEnemies = LockOnComp->FindEnemiesInRadius(1000.0f);
		if (NearbyEnemies.Num() > 0)
		{
			AActor* ClosestEnemy = LockOnComp->FindClosestEnemy(NearbyEnemies);
			if (ClosestEnemy)
			{
				LockOnComp->RotateTowardsTarget(ClosestEnemy, GetWorld()->GetDeltaSeconds());
			}
		}
	}
}

void URangeCombatComponentRH::ReEnableMovementAfterPrimaryFire()
{
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		if (!bIsCharging)
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void URangeCombatComponentRH::ReEnableMovementAfterChargeShot()
{
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargeShotMovementTimerHandle);
	}
}
