// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacters/Enums/EDamageTypesRH.h"
#include "ProjectileBaseRH.generated.h"

UCLASS()
class CS2PROJECT_API AProjectileBaseRH : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBaseRH();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TEnumAsByte<EDamageTypesRH> DamageType;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float Damage = 0.0f;

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

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse, const FHitResult& Hit);
};
