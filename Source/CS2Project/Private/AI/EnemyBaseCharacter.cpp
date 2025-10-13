// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyBaseCharacter.h"

#include "AI/EnemyAIControllerRH.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(200.0f, 10.0f));
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
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
