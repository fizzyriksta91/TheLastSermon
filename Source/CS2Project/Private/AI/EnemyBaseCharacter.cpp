// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyBaseCharacter.h"

#include "AI/EnemyAIControllerRH.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBaseCharacter::OnDeath()
{
	Super::OnDeath();

	AEnemyAIControllerRH* EnemyController = Cast<AEnemyAIControllerRH>(GetController());
	if (EnemyController)
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"),true);
		EnemyController->UnPossess();
		EnemyController->SetDeadState();
	}
}

float AEnemyBaseCharacter::SetMovementSpeed(EMovementSpeedRH SpeedType)
{
	float Speed = 0.0f;
	switch (SpeedType)
	{
	case EMovementSpeedRH::Idle:
		Speed = 0.0f;
		break;
	case EMovementSpeedRH::Walk:
		Speed = 100.0f;
		break;
	case EMovementSpeedRH::Jog:
		Speed = 300.0f;
		break;
	case EMovementSpeedRH::Sprint:
		Speed = 500.0f;
		break;
	}
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	return Speed;
}

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
