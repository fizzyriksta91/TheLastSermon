// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacters/Enums/EAISenseRH.h"
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
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI")
	AActor* AttackTarget = nullptr;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyStatesRH> InitialState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	
	void SetDeadState();
	
	void SetIdleState();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	void InitializeBlackboard();

	EEnemyStatesRH GetCurrentState() const;

	void SetAttackingState();
	
	void HandleSensedSight(AActor* Actor);

	void HandleSensedDamage(AActor* Actor);

	bool CanSenseActor(AActor* Actor, EAISenseRH Sense) const;
	
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	void StartBehaviorTreeDeferred();
	
	FTimerHandle BehaviorTreeStartTimer;

	bool bBehaviorTreeStarted = false;

	bool IsValidTarget(AActor* Actor) const;
};
