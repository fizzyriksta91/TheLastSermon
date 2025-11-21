// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacters/BaseCharacter.h"

#include "MultiplayerGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactables/InteractableRH.h"
#include "Perception/AISense_Damage.h"
#include "PlayerCharacters/Components/DodgeComponentRH.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach components
	StatsComp = CreateDefaultSubobject<UStatsComponentRH>(TEXT("Stats Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponentRH>(TEXT("Trace Component"));
	DodgeComp = CreateDefaultSubobject<UDodgeComponentRH>(TEXT("Dodge Component"));

	GetCharacterMovement()->SetWalkableFloorAngle(25.f);
} 

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::FaceMovementDirection()
{
	FVector Velocity = GetVelocity();

	if (!Velocity.IsNearlyZero(10.0f))
	{
		Velocity.Z = 0.0f;
		FRotator TargetRotation = Velocity.Rotation();
		FRotator NewRotation = FMath::RInterpTo(
			GetActorRotation(),
			TargetRotation,
			GetWorld()->GetDeltaSeconds(),
			20.0f
		);
		SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
	}
}

// Function to handle character destruction
void ABaseCharacter::DestroyCharacter()
{
	Destroy();
}

void ABaseCharacter::LockMovement()
{
	if (bMovementLocked)
		return;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}

	bMovementLocked = true;
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
float ABaseCharacter::GetDamage_Implementation(EDamageTypesRH DamageType)
{
	float Strength = StatsComp->Stats[EStatsRH::Strength];
	float Multiplier =2.0f;

	if (DamageValues.Contains(DamageType))
	{
		Multiplier = DamageValues[DamageType];
	}
	return Strength * Multiplier;
}

// Handle taking damage and report to AI perception system
float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (DodgeComp && DodgeComp->bIsInvulnerable)
	{
		return 0.0f; // No damage taken if invulnerable
	}

	float ActualDamage = Super::TakeDamage(
		DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage from %s (Instigator: %s)"),
		*GetName(), ActualDamage,
		DamageCauser ? *DamageCauser->GetName() : TEXT("NULL"),
		EventInstigator ? *EventInstigator->GetName() : TEXT("NULL"));

	// Report damage to AI perception system
	UAISense_Damage::ReportDamageEvent(
		GetWorld(), this, DamageCauser, ActualDamage,
		GetActorLocation(),GetActorLocation());

	// checks of StatsComp is valid and health is 0 or below, then calls OnDeath
	if (StatsComp && StatsComp->Stats[EStatsRH::Health] <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

// Handle character death
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
		
		if (UWidgetComponent* WidgetComp = FindComponentByClass<UWidgetComponent>())
		{
			WidgetComp->DestroyComponent();
		}
		
		// Set a timer to destroy the character after DeathDelay seconds
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this,
			&ABaseCharacter::DestroyCharacter, DeathDelay, false);
	}

	if (HasAuthority() && GetWorld())
	{
		if (AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (IsBoss())
			{
				GameMode->OnBossDefeated();
			}

			GameMode->NotifyCharacterDeath(this);
		}
	}
}
// Check if the character is dead
bool ABaseCharacter::IsDead() const
{
	return StatsComp && StatsComp->Stats[EStatsRH::Health] <= 0.0f;
}

// Get the character's health as a percentage of max health for health bar UI, needs to be fixed
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

void ABaseCharacter::HealToFull()
{
	if (StatsComp)
	{
		StatsComp->Stats[EStatsRH::Health] = StatsComp->Stats[EStatsRH::MaxHealth];
		UE_LOG(LogTemp, Warning, TEXT("%s healed to full health!"), *GetName());
	}
}

void ABaseCharacter::TryInteract()
{
	if (!GetCapsuleComponent())
	{
		UE_LOG(LogTemp, Error, TEXT("CapsuleComponent is null!"));
		return;
	}

	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	float ClosestDistance = InteractionDistance;
	AActor* ClosestInteractable = nullptr;

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || !IsValid(Actor) || Actor == this)
		{
			continue;
		}
		
		if (Actor && Actor->GetClass()->ImplementsInterface(UInteractableRH::StaticClass()))
		{
			float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestInteractable = Actor;
			}
		}
	}

	if (ClosestInteractable->ActorHasTag(FName(TEXT("Dialogue"))))
	{
		LockMovement();
		
		IInteractableRH::Execute_Interact(ClosestInteractable, this);
		UE_LOG(LogTemp, Warning, TEXT("%s interacted with %s"), *GetName(), *ClosestInteractable->GetName());
		return;
	}
	
	IInteractableRH::Execute_Interact(ClosestInteractable, this);
}

void ABaseCharacter::UnlockMovement()
{
	if (!bMovementLocked)
		return;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	bMovementLocked = false;
}







