// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/ToggleTraceNotifyStateRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

// Notify when the animation starts
void UToggleTraceNotifyStateRH::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// Ensure MeshComp and Owner are valid
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	// Find the Trace Component
	UTraceComponentRH* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponentRH>() };

	// Ensure TraceComp is valid and set attacking to true
	if (!IsValid(TraceComp)) { return; }
	
	TraceComp->bIsAttacking = true;
}

// Notify when the animation ends
void UToggleTraceNotifyStateRH::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	UTraceComponentRH* TraceComp{ MeshComp->GetOwner()->FindComponentByClass<UTraceComponentRH>() };

	// Ensure TraceComp is valid and set attacking to false
	if (!IsValid(TraceComp)) { return; }
	TraceComp->bIsAttacking = false;
}
