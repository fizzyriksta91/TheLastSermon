// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_KickComboAttackRH.h"
#include "AI/EnemyAIControllerRH.h"
#include "AI/BossCharacterRH.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UBTT_KickComboAttackRH::UBTT_KickComboAttackRH()
{
	NodeName = TEXT("Kick Combo Attack (Wait For Montage)");
}

EBTNodeResult::Type UBTT_KickComboAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return EBTNodeResult::Failed;
	}

	ABossCharacterRH* BossCharacter = Cast<ABossCharacterRH>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(BossCharacter))
	{
		return EBTNodeResult::Failed;
	}
	
	
	UAnimInstance* AnimInst = BossCharacter->GetMesh() ? BossCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInst)
	{
		// If no anim instance, fall back to just performing combo and succeed immediately.
		BossCharacter->PerformKickCombo();
		return EBTNodeResult::Succeeded;
	}

	CachedOwnerComp = &OwnerComp;
	BoundAnimInstance = AnimInst;

	MontageEndDelegate.BindUObject(this, &UBTT_KickComboAttackRH::OnMontageEnded);
	BoundAnimInstance->Montage_SetEndDelegate(MontageEndDelegate);

	BossCharacter->PerformKickCombo();

	return EBTNodeResult::InProgress;
}

void UBTT_KickComboAttackRH::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (BoundAnimInstance)
	{
		MontageEndDelegate.Unbind();
		BoundAnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
		BoundAnimInstance = nullptr;
	}

	if (CachedOwnerComp)
	{
		const EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
		FinishLatentTask(*CachedOwnerComp, Result);
	}

	CachedOwnerComp = nullptr;
	BoundAnimInstance = nullptr;
}
