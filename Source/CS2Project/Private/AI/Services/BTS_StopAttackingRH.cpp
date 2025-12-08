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

// Check if the attack target is dead and update blackboard accordingly
void UBTS_StopAttackingRH::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AEnemyAIControllerRH* EnemyController = Cast<AEnemyAIControllerRH>(OwnerComp.GetAIOwner());
	if (!EnemyController || !EnemyController->GetBlackboardComponent())
		return;
	
	AActor* CurrentTarget = Cast<AActor>(
		EnemyController->GetBlackboardComponent()->GetValueAsObject(
			TEXT("AttackTarget")));

	// If there is a current target, check if it's dead
	if (CurrentTarget)
	{
		// Check if the target implements the combat interface and if it's dead
		ICombatRH* CombatInterface = Cast<ICombatRH>(CurrentTarget);
		if (CombatInterface && CombatInterface->IsDead())
		{
			// If the target is dead, clear the attack target and set the enemy to idle state
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
			EnemyController->GetBlackboardComponent()->ClearValue(TEXT("AttackTarget"));
			EnemyController->SetIdleState();
		}
	}
}
