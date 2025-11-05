// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_KickComboAttackRH.h"
#include "AI/EnemyAIControllerRH.h"
#include "AI/BossCharacterRH.h"

EBTNodeResult::Type UBTT_KickComboAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossCharacterRH* BossCharacter = Cast<ABossCharacterRH>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(BossCharacter))
	{
		return EBTNodeResult::Failed;
	}
	
	BossCharacter->PerformKickCombo();
	
	return EBTNodeResult::Succeeded;
}
