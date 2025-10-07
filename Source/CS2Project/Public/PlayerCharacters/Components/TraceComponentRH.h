// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacters/FTraceSocketsRH.h"
#include "TraceComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API UTraceComponentRH : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(EditAnywhere)
	TArray<FTraceSocketsRH> Sockets;

	UPROPERTY(EditAnywhere)
	double BoxCollisonLength{ 30.0 };

	UPROPERTY(EditAnywhere)
	bool bDebugMode{ false };

	UPROPERTY()
	TArray<AActor*> TargetsToIgnore;

public:	
	// Sets default values for this component's properties
	UTraceComponentRH();

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking{ true };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();
};
