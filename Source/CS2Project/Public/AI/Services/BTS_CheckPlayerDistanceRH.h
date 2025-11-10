// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckPlayerDistanceRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UBTS_CheckPlayerDistanceRH : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckPlayerDistanceRH();

protected:
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector DistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector bIsPlayerInRangeKey;

	UPROPERTY(EditAnywhere, Category = "Config", meta = (ClampMin = "0.0"))
	float MeleeRangeThreshold = 100.f;
};
