// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SelectRandomPlayer_RH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UBTS_SelectRandomPlayer_RH : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_SelectRandomPlayer_RH();
	
	UPROPERTY(EditAnywhere, Category = "Search")
	float SearchRadius { 2000.f };
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
