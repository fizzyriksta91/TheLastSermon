// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_SelectRandomPlayer_RH.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTS_SelectRandomPlayer_RH::UBTS_SelectRandomPlayer_RH()
{
	NodeName = "Select Random Player In Range";
	Interval = 1.0f;
	RandomDeviation = 0.2f;
}

// Select a random player pawn within the search radius and update the blackboard
void UBTS_SelectRandomPlayer_RH::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
	
	APawn* OwnerPawn = AIController->GetPawn();
	if (!OwnerPawn) return;
	
	UWorld* World = OwnerPawn->GetWorld();
	if (!World) return;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), FoundActors);
	
	TArray<APawn*> ValidTargets;
	const float SearchRadiusSqr = SearchRadius * SearchRadius;
	
	// Filter actors within the search radius
	for (AActor* Actor : FoundActors)
	{
		APawn* TargetPawn = Cast<APawn>(Actor);
		if (!TargetPawn || TargetPawn == OwnerPawn) continue;
		
		if (FVector::DistSquared(TargetPawn->GetActorLocation(), OwnerPawn->GetActorLocation()) <= SearchRadiusSqr)
		{
			ValidTargets.Add(TargetPawn);
		}
	}
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;
	
	// Select a random target from valid targets
	if (ValidTargets.Num() > 0)
	{
		APawn* ChosenPawn = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
		Blackboard->SetValueAsObject(AttackTargetKey.SelectedKeyName, ChosenPawn);
	}
	else
	{
		Blackboard->ClearValue(AttackTargetKey.SelectedKeyName);
	}
}
