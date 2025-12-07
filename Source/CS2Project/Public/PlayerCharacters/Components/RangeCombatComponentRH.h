// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangeCombatComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API URangeCombatComponentRH : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* PrimaryAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* ChargeAttackMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* ChargeStartMontage;
	
	FTimerHandle PrimaryFireSpawnTimerHandle;
	
	FTimerHandle ChargeShotSpawnTimerHandle;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* PrimaryFireSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ChargeStartSound = nullptr;

private:
	UPROPERTY()
	ACharacter* CharacterRef;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float LastShotTime { -1.0f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShotCooldown { 0.5f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChargeShotDuration { 1.675f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChargeShotCooldown { 5.0f };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float LastChargeShotTime { -10.0f };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bIsCharging { false };

	FTimerHandle ChargeShotTimerHandle;

	void FireChargeShot();

	void SpawnPrimaryProjectile();

	void SpawnChargeProjectile();

	void RotateTowardsNearestEnemy();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PrimaryFireMovementLockDuration { 0.1f };

	FTimerHandle PrimaryFireMovementTimerHandle;

	UFUNCTION()
	void ReEnableMovementAfterPrimaryFire();
	
	UFUNCTION()
	void ReEnableMovementAfterChargeShot();
	
	FTimerHandle ChargeShotMovementTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChargeAttackMovementLockDuration { 0.1f };
};
