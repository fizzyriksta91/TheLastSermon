// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyBaseCharacter.h"

#include "AI/EnemyAIControllerRH.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	
}


void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Handle death logic specific to the enemy character
void AEnemyBaseCharacter::OnDeath()
{
	Super::OnDeath();

	// Notify the AI controller of death
	AEnemyAIControllerRH* EnemyController = Cast<AEnemyAIControllerRH>(GetController());
	
	// Ensure the controller is valid before accessing its blackboard
	if (EnemyController)
	{
		// Set the "IsDead" key in the blackboard to true, unpossess the character and change state to Dead
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);
		EnemyController->UnPossess();
		EnemyController->SetDeadState();
	}
}

// Set the enemy's movement speed based on the specified speed type
float AEnemyBaseCharacter::SetMovementSpeed(EMovementSpeedRH SpeedType)
{
	float Speed = 0.0f;
	switch (SpeedType)
	{
	case EMovementSpeedRH::Idle:
		Speed = 0.0f;
		break;
	case EMovementSpeedRH::Walk:
		Speed = 200.0f;
		break;
	case EMovementSpeedRH::Jog:
		Speed = 400.0f;
		break;
	case EMovementSpeedRH::Sprint:
		Speed = 600.0f;
		break;
	}
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	return Speed;
}

// Perform a melee attack by playing the attack animation and setting the damage type
void AEnemyBaseCharacter::PerformMeleeAttack()
{
	if (!MeleeAttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("No BasicAttackMontage set for enemy"));
		return;
	}

	// Set damage type
	if (TraceComp)
	{
		TraceComp->SetCurrentDamageType(EDamageTypesRH::LightAttack);
		TraceComp->HandleResetAttack();
	}

	// Play attack animation
	PlayAnimMontage(MeleeAttackMontage);
}
