// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_BasicMeleeAttackRH.h"
#include "AI/EnemyAIControllerRH.h"
#include "AI/EnemyBaseCharacter.h"


EBTNodeResult::Type UBTT_BasicMeleeAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the enemy character from the AI controller
	AEnemyBaseCharacter* EnemyCharacter = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	// Ensure the enemy character is valid
	if (!IsValid(EnemyCharacter))
	{
		return EBTNodeResult::Failed;
	}
	// Perform the melee attack
	EnemyCharacter->PerformMeleeAttack();

	return EBTNodeResult::Succeeded;
}
