// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangeCombatComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API URangeCombatComponentRH : public UActorComponent
{
	GENERATED_BODY()

	void RotateTowardsNearestEnemy();

public:	
	// Sets default values for this component's properties
	URangeCombatComponentRH();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(
		float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void PerformPrimaryRangedAttack();

	UFUNCTION(BlueprintCallable)
	void StartChargeShot();

	UFUNCTION(BlueprintCallable)
	void CancelChargeShot();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	bool IsCharging() const { return bIsCharging; }

private:
	UPROPERTY()
	ACharacter* CharacterRef;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float LastShotTime { -1.0f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShotCooldown { 0.5f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChargeShotDuration { 2.0f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChargeShotCooldown { 5.0f };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float LastChargeShotTime { -10.0f };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bIsCharging { false };

	FTimerHandle ChargeShotTimerHandle;

	void FireChargeShot();
};
