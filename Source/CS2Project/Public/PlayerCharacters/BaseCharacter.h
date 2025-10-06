// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatRH.h"
#include "BaseCharacter.generated.h"

UCLASS()
class CS2PROJECT_API ABaseCharacter : public ACharacter , public ICombatRH
{
	GENERATED_BODY()

	

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponentRH* StatsComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponentRH* TraceComp;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Makes the character face its movement direction
	void FaceMovementDirection();

	FRotator LastMovementRotation;

	bool bHasMovedBefore { false };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;
	
};
