// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_StopAttackingRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UBTS_StopAttackingRH : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_StopAttackingRH();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
