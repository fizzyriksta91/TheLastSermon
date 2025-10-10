// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIControllerRH.h"

#include "AI/EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemyAIControllerRH::AEnemyAIControllerRH()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AEnemyAIControllerRH::BeginPlay()
{
	Super::BeginPlay();

	AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(GetPawn());
	if (Enemy && BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	
	BlackboardComp = GetBlackboardComponent();
	
	InitializeBlackboard();
	
}

void AEnemyAIControllerRH::InitializeBlackboard()
{
	if (!BlackboardComp)
		return;
	
	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);

	APawn* PlayerPawn0 = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn* PlayerPawn1 = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);

	APawn* ClosestPawn = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	APawn* EnemyPawn = GetPawn();
	if (!EnemyPawn)
		return;
		
	if (PlayerPawn0)
	{
		float Distance = FVector::Dist(
			EnemyPawn->GetActorLocation(), PlayerPawn0->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestPawn = PlayerPawn0;
		}
	}
	if (PlayerPawn1)
	{
		float Distance = FVector::Dist(
			EnemyPawn->GetActorLocation(), PlayerPawn1->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestPawn = PlayerPawn1;
		}
	}
	if (ClosestPawn)
	{
		BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), ClosestPawn);
	}
}


