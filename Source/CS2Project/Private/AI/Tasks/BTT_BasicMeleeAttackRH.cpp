// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_BasicMeleeAttackRH.h"

EBTNodeResult::Type UBTT_BasicMeleeAttackRH::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Basic Melee Attack Executed"));

	return EBTNodeResult::Succeeded;
}
