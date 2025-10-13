// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_StopAttackingRH.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAIControllerRH.h"
#include "PlayerCharacters/Interfaces/CombatRH.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


UBTS_StopAttackingRH::UBTS_StopAttackingRH()
{
	NodeName = TEXT("Stop Attacking Service");
	Interval = 0.5f; // Check every 0.5 seconds
	RandomDeviation = 0.0f;
}

void UBTS_StopAttackingRH::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIControllerRH* EnemyController = Cast<AEnemyAIControllerRH>(OwnerComp.GetAIOwner());
	if (!EnemyController || !EnemyController->GetBlackboardComponent())
		return;

	AActor* CurrentTarget = Cast<AActor>(
		EnemyController->GetBlackboardComponent()->GetValueAsObject(
			TEXT("AttackTarget")));

	if (CurrentTarget)
	{
		ICombatRH* CombatInterface = Cast<ICombatRH>(CurrentTarget);
		if (CombatInterface && CombatInterface->IsDead())
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
			EnemyController->GetBlackboardComponent()->ClearValue(TEXT("AttackTarget"));

			EnemyController->SetIdleState();
		}
	}
}
