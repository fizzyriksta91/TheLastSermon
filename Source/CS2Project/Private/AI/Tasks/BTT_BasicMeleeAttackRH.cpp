// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_BasicMeleeAttackRH.h"
#include "AI/EnemyAIControllerRH.h"
#include "AI/EnemyBaseCharacter.h"

// Execute the basic melee attack task
EBTNodeResult::Type UBTT_BasicMeleeAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyBaseCharacter* EnemyCharacter = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!IsValid(EnemyCharacter))
		return EBTNodeResult::Failed;
	
	
	EnemyCharacter->PerformMeleeAttack();

	return EBTNodeResult::Succeeded;
}
