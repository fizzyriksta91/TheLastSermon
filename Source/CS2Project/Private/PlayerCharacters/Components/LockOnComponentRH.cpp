// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacters/Interfaces/EnemyRH.h"

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
	OwnerRef = GetOwner<ACharacter>();
	
}

// Called every frame
void ULockOnComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void ULockOnComponentRH::StartLockon(float Radius)
{
	if (!IsValid(OwnerRef)) { return; }

	
	TArray<FHitResult> OutResults;
	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius)};
	FCollisionQueryParams IgnoreParams(
		TEXT("LockOnTrace"), false, OwnerRef);

	IgnoreParams.AddIgnoredActor(OwnerRef);

	bool bHasFoundTarget{ GetWorld()->SweepMultiByChannel(
		OutResults, CurrentLocation, CurrentLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1, Sphere, IgnoreParams) };

	if (bHasFoundTarget)
	{
		for (const FHitResult& Hit : OutResults)
		{
			AActor* FoundActor = Hit.GetActor();

			if (IsValid(FoundActor) && FoundActor->Implements<UEnemyRH>())
			{

				ACharacter* CharacterRef = Cast<ACharacter>(FoundActor);
				if (!CharacterRef || !CharacterRef->IsPlayerControlled())
				{
					CurrentTargetActor = FoundActor;
					UE_LOG(LogTemp, Log, TEXT("LockOn: Found enemy - %s"), *CurrentTargetActor->GetName());
					OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
					return;
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("LockOn: Found actors but none are valid enemies"));
}




