// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacters/Enums/EEnemyStatesRH.h"
#include "EnemyAIControllerRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API AEnemyAIControllerRH : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIControllerRH();

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyStatesRH> InitialState;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	
};
