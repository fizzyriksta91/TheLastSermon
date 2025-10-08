// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeCombatComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API UMeleeCombatComponentRH : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimMontage*> LightComboMontages;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY()
	ACharacter* CharacterRef;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int ComboCounter { 0 };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bCanAttack { true };

	FTimerHandle ComboResetTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ComboResetDelay { 1.5f };

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float LastHeavyAttackTime { 0.0f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float HeavyAttackCooldown { 10.0f };

public:	
	// Sets default values for this component's properties
	UMeleeCombatComponentRH();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PerformLightComboAttack();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	void ResetCombatCounter();

	UFUNCTION(BlueprintCallable)
	void PerformHeavyAttack();
	
	

	
};
