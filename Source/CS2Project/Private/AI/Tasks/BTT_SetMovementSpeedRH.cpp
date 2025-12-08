// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SetMovementSpeedRH.h"

#include "AI/EnemyAIControllerRH.h"
#include "PlayerCharacters/Interfaces/EnemyRH.h"

EBTNodeResult::Type UBTT_SetMovementSpeedRH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIControllerRH* AIController = Cast<AEnemyAIControllerRH>(OwnerComp.GetAIOwner());
	
	if (!AIController)
		return EBTNodeResult::Failed;
	
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
		return EBTNodeResult::Failed;

	// Cast the pawn to the enemy interface and set the movement speed
	IEnemyRH* EnemyInterface = Cast<IEnemyRH>(Pawn);
	if (EnemyInterface)
	{
		// Set the movement speed using the value from the blackboard
		EnemyInterface->SetMovementSpeed(CurrentMovementSpeed);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
