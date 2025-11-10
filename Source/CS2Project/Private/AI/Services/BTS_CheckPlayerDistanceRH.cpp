// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_CheckPlayerDistanceRH.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTS_CheckPlayerDistanceRH::UBTS_CheckPlayerDistanceRH()
{
	bNotifyTick = true;
	NodeName = TEXT("Check Player Distance");
}

void UBTS_CheckPlayerDistanceRH::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FVector CurrentLocation =
		OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(
		GetWorld(), 0)->GetActorLocation();

	float Distance = static_cast<float>(FVector::Distance(CurrentLocation, PlayerLocation));
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
		TEXT("Distance"), Distance);
}
