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

// Execute the kick combo attack task
EBTNodeResult::Type UBTT_KickComboAttackRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	
	if (!AICon)
		return EBTNodeResult::Failed;

	ABossCharacterRH* BossCharacter = Cast<ABossCharacterRH>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!IsValid(BossCharacter))
		return EBTNodeResult::Failed;
	
	// Get the animation instance to bind to montage end event
	UAnimInstance* AnimInst = BossCharacter->GetMesh() ? BossCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInst)
	{
		BossCharacter->PerformKickCombo();
		return EBTNodeResult::Succeeded;
	}
	
	// Bind to montage end event and perform the kick combo
	CachedOwnerComp = &OwnerComp;
	BoundAnimInstance = AnimInst;
	MontageEndDelegate.BindUObject(this, &UBTT_KickComboAttackRH::OnMontageEnded);
	BoundAnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
	BossCharacter->PerformKickCombo();

	return EBTNodeResult::InProgress;
}

// Handle montage end event
void UBTT_KickComboAttackRH::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Unbind the delegate to avoid dangling references
	if (BoundAnimInstance)
	{
		MontageEndDelegate.Unbind();
		BoundAnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
		BoundAnimInstance = nullptr;
	}

	// Finish the latent task based on whether the montage was interrupted
	if (CachedOwnerComp)
	{
		const EBTNodeResult::Type Result = bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
		FinishLatentTask(*CachedOwnerComp, Result);
	}
	
	// Clear cached references
	CachedOwnerComp = nullptr;
	BoundAnimInstance = nullptr;
}
