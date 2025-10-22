// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponentRH.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature, 
	ULockOnComponentRH, OnUpdatedTargetDelegate, 
	AActor*, NewTargetActorRef);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API ULockOnComponentRH : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponentRH();

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdatedTargetDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Lock On")
	TArray<AActor*> FindEnemiesInRadius(float Radius);

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Lock On")
	float DetectionRadius { 1000.0f };
};
