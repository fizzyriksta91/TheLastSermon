// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/RangeCombatComponentRH.h"
#include "GameFramework/Character.h"
#include "PlayerCharacters/ProjectileBaseRH.h"
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

	// ...
}

void URangeCombatComponentRH::PerformPrimaryRangedAttack()
{
	// Check Cooldown
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastShotTime < ShotCooldown)
	{
		return; // Still in cooldown
	}

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

		// Calculate damage using the character's GetDamage method
		if (ICombatRH* CombatInterface = Cast<ICombatRH>(CharacterRef))
		{
			Projectile->Damage = CombatInterface->GetDamage(EDamageTypesRH::GunShot);
		}
	}
	LastShotTime = CurrentTime;
}

