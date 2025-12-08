// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/LockOnComponentRH.h"

#include "GameFramework/Character.h"
#include "PlayerCharacters/BaseCharacter.h"


// Sets default values for this component's properties
ULockOnComponentRH::ULockOnComponentRH()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void ULockOnComponentRH::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

// Called every frame
void ULockOnComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Find enemies within a specified radius and lock on
TArray<AActor*> ULockOnComponentRH::FindEnemiesInRadius(float Radius)
{
	TArray<AActor*> FoundEnemies;

	if (!OwnerCharacter)
	{
		return FoundEnemies;
	}

	TArray<FHitResult> HitResults;
	FVector StartLocation = OwnerCharacter->GetActorLocation();

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	// Perform the sphere sweep to find enemies
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, StartLocation, StartLocation,
		FQuat::Identity, ECC_Pawn, Sphere, QueryParams);
	
	if (bHit)
	{
		// Process hit results to find valid enemies
		for (const FHitResult& Hit : HitResults)
		{
			if (ABaseCharacter* Enemy = Cast<ABaseCharacter>(Hit.GetActor()))
			{
				if (!Enemy->IsDead() && Enemy->GetController()
					&& !Enemy->GetController()->IsPlayerController())
				{
					FoundEnemies.Add(Enemy);
				}
			}
		}
	}
	return FoundEnemies;
}

// Find the closest enemy from a list
AActor* ULockOnComponentRH::FindClosestEnemy(const TArray<AActor*>& Enemies)
{
	if (Enemies.Num() == 0 || !OwnerCharacter) { return nullptr; }

	AActor* ClosestEnemy = nullptr;
	float ClosestDistance = FLT_MAX;
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	
	// Iterate through enemies to find the closest one
	for (AActor* Enemy : Enemies)
	{
		float Distance = FVector::Dist(OwnerLocation, Enemy->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestEnemy = Enemy;
		}
	}

	return ClosestEnemy;
}

// Rotate the owner character to face the target actor
void ULockOnComponentRH::RotateTowardsTarget(AActor* Target, float DeltaTime)
{
	if (!Target || !OwnerCharacter) 
		return;
	
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector Direction = TargetLocation - OwnerLocation;
	Direction.Z = 0.0f;

	// Smoothly interpolate rotation towards the target
	if (!Direction.IsNearlyZero())
	{
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(
			OwnerCharacter->GetActorRotation(),
			TargetRotation, DeltaTime, 100.f);

		OwnerCharacter->SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	}
}





