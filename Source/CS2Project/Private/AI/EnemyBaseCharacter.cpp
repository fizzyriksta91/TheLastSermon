// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyBaseCharacter.h"

#include "AI/EnemyAIControllerRH.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"
#include "UI/HealthBarRH.h"


void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Handle death logic specific to the enemy character
void AEnemyBaseCharacter::OnDeath()
{
	Super::OnDeath();
	
	AEnemyAIControllerRH* EnemyController = Cast<AEnemyAIControllerRH>(GetController());
	
	// Ensure the controller is valid before accessing its blackboard
	if (EnemyController)
	{
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
		Speed = 700.0f;
		break;
	}
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	return Speed;
}

// Perform a melee attack by playing the attack animation and setting the damage type
void AEnemyBaseCharacter::PerformMeleeAttack()
{
	if (!MeleeAttackMontage)
		return;

	// Set damage type for the trace component
	if (TraceComp)
	{
		TraceComp->SetCurrentDamageType(EDamageTypesRH::LightAttack);
		TraceComp->HandleResetAttack();
	}
	
	PlayAnimMontage(MeleeAttackMontage);
}
