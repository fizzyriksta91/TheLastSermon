// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayerCharacters/Enums/EMovementSpeedRH.h"
#include "BTT_SetMovementSpeedRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UBTT_SetMovementSpeedRH : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementSpeedRH> CurrentMovementSpeed;
	
};
