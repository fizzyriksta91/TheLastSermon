// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBaseRH.generated.h"

UCLASS()
class CS2PROJECT_API AProjectileBaseRH : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBaseRH();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComp;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
