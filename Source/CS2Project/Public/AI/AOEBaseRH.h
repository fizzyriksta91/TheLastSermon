// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "AOEBaseRH.generated.h"

UCLASS()
class CS2PROJECT_API AAOEBaseRH : public AActor
{
	GENERATED_BODY()

public:
	AAOEBaseRH();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Radius used for the overlap and debug draw
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float Radius = 200.f;

	// Draw debug sphere when spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	bool bDrawDebugSphere = true;

	// If true, will add the Instigator to the ActorsToIgnore list
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	bool bIgnoreInstigator = true;

	// Object types to query (defaults to Pawn)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// Spawn the AOE sphere (BlueprintCallable so can be invoked from BP)
	UFUNCTION(BlueprintCallable, Category = "AOE")
	void SpawnAOESphere();

	// Called for each overlapped Actor; overridable in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "AOE")
	void OnAOEOverlapActor(AActor* OtherActor);
	virtual void OnAOEOverlapActor_Implementation(AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	bool bTriggerOnBeginPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE", meta = (ClampMin = "0.0"))
	float DelayOnBeginPlay = 0.2f;

	UFUNCTION(BlueprintCallable, Category = "AOE")
	void Trigger();

private:
	FTimerHandle TriggerBeginPlayHandle;
};