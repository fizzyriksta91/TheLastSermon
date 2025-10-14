// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/ToggleTraceNotifyStateRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

void UToggleTraceNotifyStateRH::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UTraceComponentRH* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponentRH>() };

	if (!IsValid(TraceComp)) { return; }

	TraceComp->bIsAttacking = true;
}

void UToggleTraceNotifyStateRH::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UTraceComponentRH* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponentRH>() };

	if (!IsValid(TraceComp)) { return; }

	TraceComp->bIsAttacking = false;
}
