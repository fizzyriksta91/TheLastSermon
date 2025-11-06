// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossCharacterRH.h"

#include "PlayerCharacters/Components/TraceComponentRH.h"

void ABossCharacterRH::PerformKickCombo()
{
	if (!KickComboAnimMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("No KickComboAnimMontage"));
		return;
	}

	if (TraceComp)
	{
		TraceComp->SetCurrentDamageType(EDamageTypesRH::KickAttack);
		TraceComp->HandleResetAttack();
	}
	
	PlayAnimMontage(KickComboAnimMontage);
}

void ABossCharacterRH::SetDamageType(TEnumAsByte<EDamageTypesRH> NewType)
{
	DamageType = NewType;
}
