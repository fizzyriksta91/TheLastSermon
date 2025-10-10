// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_BasicMeleeAttackRH.h"
#include "AI/EnemyAIControllerRH.h"
#include "GameFramework/Character.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

EBTNodeResult::Type UBTT_BasicMeleeAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>() };

	CurrentDamageType = EDamageTypesRH::LightAttack;

	if (CharacterRef)
	{
		auto TraceComp = CharacterRef->FindComponentByClass<UTraceComponentRH>();
		if (TraceComp)
		{
			TraceComp->SetCurrentDamageType(CurrentDamageType);
			TraceComp->HandleResetAttack();
		}
	}

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; }
	CharacterRef->PlayAnimMontage(BasicAttackMontage);

	return EBTNodeResult::Succeeded;
}
