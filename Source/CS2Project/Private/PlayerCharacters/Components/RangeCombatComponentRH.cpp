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
		float SpawnDelay = FMath::Min(MontageDuration, 0.1f); 
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

	// Start timer to fire charge shot
	GetWorld()->GetTimerManager().SetTimer(
		ChargeShotTimerHandle, this, &URangeCombatComponentRH::FireChargeShot,
		ChargeShotDuration, false);

	UE_LOG(LogTemp, Display, TEXT("Charging shot"));
}

void URangeCombatComponentRH::CancelChargeShot()
{
	if (!bIsCharging) { return; }

	// Re-enable character movement
	if (ACharacter* Character = Cast<ACharacter>(CharacterRef))
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	// Clear Timer
	GetWorld()->GetTimerManager().ClearTimer(ChargeShotTimerHandle);

	bIsCharging = false;
	UE_LOG(LogTemp, Display, TEXT("Charge shot cancelled"));
}

void URangeCombatComponentRH::FireChargeShot()
{
	if (!ProjectileClass || !CharacterRef) return;

	if (ChargeAttackMontage && CharacterRef)
	{
		float MontageDuration = CharacterRef->PlayAnimMontage(ChargeAttackMontage);
		float SpawnDelay = FMath::Min(MontageDuration, 0.1f); // adjust to sync with animation
		GetWorld()->GetTimerManager().SetTimer(
			ChargeShotSpawnTimerHandle, this,
			&URangeCombatComponentRH::SpawnChargeProjectile,
			SpawnDelay, false);
	}
	else
	{
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
		Projectile->DamageType = EDamageTypesRH::ChargeShot;

		if (ICombatRH* CombatInterface = Cast<ICombatRH>(CharacterRef))
		{
			Projectile->Damage = CombatInterface->GetDamage(EDamageTypesRH::ChargeShot);
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
		CharacterRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Projectile->DamageType = EDamageTypesRH::ChargeShot;
		if (ICombatRH* CombatInterface = Cast<ICombatRH>(CharacterRef))
		{
			Projectile->Damage = CombatInterface->GetDamage(EDamageTypesRH::ChargeShot);
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
