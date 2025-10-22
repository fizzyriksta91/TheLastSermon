// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/LockOnComponentRH.h"

#include "GameFramework/Character.h"
#include "PlayerCharacters/BaseCharacter.h"


// Sets default values for this component's properties
ULockOnComponentRH::ULockOnComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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

TArray<AActor*> ULockOnComponentRH::FindEnemiesInRadius(float Radius)
{
	TArray<AActor*> FoundEnemies;

	if (!OwnerCharacter) { return FoundEnemies; }

	TArray<FHitResult> HitResults;
	FVector StartLocation = OwnerCharacter->GetActorLocation();

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, StartLocation, StartLocation,
		FQuat::Identity, ECC_Pawn, Sphere, QueryParams);

	DrawDebugSphere(GetWorld(), StartLocation, Radius, 32, FColor::Green, false, 2.0f);
	
	if (bHit)
	{
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

	UE_LOG(LogTemp, Warning, TEXT(
		"Found %d Enemies within radius %.2f"), FoundEnemies.Num(), Radius);
	return FoundEnemies;
}





