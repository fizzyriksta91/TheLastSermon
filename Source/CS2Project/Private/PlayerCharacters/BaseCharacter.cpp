// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "PlayerCharacters/Components/DodgeComponentRH.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponentRH>(TEXT("Stats Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponentRH>(TEXT("Trace Component"));
	DodgeComp = CreateDefaultSubobject<UDodgeComponentRH>(TEXT("Dodge Component"));
	
} 

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::FaceMovementDirection()
{
	// Get the character's velocity
	FVector Velocity = GetVelocity();
    
	// Only rotate if we're actually moving
	if (!Velocity.IsNearlyZero())
	{
		// Zero out Z component to keep rotation on XY plane
		Velocity.Z = 0.0f;
		Velocity.Normalize();
        
		// Convert to rotation and set actor rotation (only yaw component)
		FRotator NewRotation = Velocity.Rotation();
		LastMovementRotation = FRotator(0.f, NewRotation.Yaw, 0.f);
		SetActorRotation(LastMovementRotation);
		bHasMovedBefore = true;
	}
	else if (bHasMovedBefore)
	{
		// If not moving, maintain the last known movement direction
		SetActorRotation(LastMovementRotation);
	}
}

void ABaseCharacter::DestroyCharacter()
{
	Destroy();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Auto-face the movement direction
	FaceMovementDirection();

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

// Returns the damage value based on the character's strength and the damage type multiplier
float ABaseCharacter::GetDamage(EDamageTypesRH DamageType)
{
	float Strength = StatsComp->Stats[EStatsRH::Strength];
	float Multiplier =2.0f;

	if (DamageValues.Contains(DamageType))
	{
		Multiplier = DamageValues[DamageType];
	}
	return Strength * Multiplier;
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(
		DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAISense_Damage::ReportDamageEvent(
		GetWorld(), this, DamageCauser, ActualDamage,
		GetActorLocation(),GetActorLocation());
	
	if (StatsComp && StatsComp->Stats[EStatsRH::Health] <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ABaseCharacter::OnDeath()
{
	OnCharacterDeath.Broadcast(this);

	// If this character is player-controlled, enable ragdoll physics and disable movement
	if (GetController() && GetController()->IsPlayerController())
	{
		// Enable ragdoll physics
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		GetMesh()->SetSimulatePhysics(true);
		
		// Disable character movement and collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		DisableInput(nullptr);
		
		GetCharacterMovement()->DisableMovement();

		// optional to add Revive or respawn logic.
		UE_LOG(LogTemp, Warning, TEXT("Player character died, Waiting For Revive"));
	}
	// If this character is AI-controlled, enable ragdoll physics and set a timer to destroy the character
	else
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		GetMesh()->SetSimulatePhysics(true);
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// Set a timer to destroy the character after DeathDelay seconds
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this,
			&ABaseCharacter::DestroyCharacter, DeathDelay, false);
	}
}

bool ABaseCharacter::IsDead() const
{
	return StatsComp && StatsComp->Stats[EStatsRH::Health] <= 0.0f;
}

float ABaseCharacter::GetHealthPercent() const
{
	if (StatsComp)
	{
		float CurrentHealth = StatsComp->Stats[EStatsRH::Health];
		float MaxHealth = StatsComp->Stats[EStatsRH::MaxHealth];
		return (MaxHealth > 0.0f) ? CurrentHealth / MaxHealth : 0.0f;
	}
	return 0.0f;
}







