// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIControllerRH.h"

#include "AI/EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


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
	
	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);
	
}


