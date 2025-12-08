// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossCharacterRH.h"

#include "PlayerCharacters/Components/TraceComponentRH.h"

// perform a kick combo attack
void ABossCharacterRH::PerformKickCombo()
{
	if (!KickComboAnimMontage)
		return;

	// Set damage type for the trace component
	if (TraceComp)
	{
		TraceComp->SetCurrentDamageType(EDamageTypesRH::KickAttack);
		TraceComp->HandleResetAttack();
	}
	
	PlayAnimMontage(KickComboAnimMontage);
}

// set the damage type for the boss character
void ABossCharacterRH::SetDamageType(TEnumAsByte<EDamageTypesRH> NewType)
{
	DamageType = NewType;
}
