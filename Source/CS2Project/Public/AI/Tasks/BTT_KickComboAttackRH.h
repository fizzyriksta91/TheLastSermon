// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayerCharacters/Enums/EDamageTypesRH.h"
#include "BTT_KickComboAttackRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UBTT_KickComboAttackRH : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EDamageTypesRH> CurrentDamageType { EDamageTypesRH::None };

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
