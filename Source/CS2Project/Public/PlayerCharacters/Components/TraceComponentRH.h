// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacters/FTraceSocketsRH.h"
#include "PlayerCharacters/Enums/EDamageTypesRH.h"
#include "TraceComponentRH.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnTraceHitSignature, AActor*, HitActor, FVector, HitLocation);


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

	UPROPERTY(EditAnywhere)
	bool bIsAttacking{ false };

	UPROPERTY()
	TEnumAsByte<EDamageTypesRH> CurrentDamageType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel { ECC_GameTraceChannel1 };
	
	UPROPERTY(BlueprintAssignable, Category = "Trace")
	FOnTraceHitSignature OnHit;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	void SetCurrentDamageType(EDamageTypesRH InType);
};
