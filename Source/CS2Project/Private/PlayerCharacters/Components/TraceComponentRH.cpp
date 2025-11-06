// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/TraceComponentRH.h"

#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacters/Interfaces/CombatRH.h"

// Sets default values for this component's properties
UTraceComponentRH::UTraceComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponentRH::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	
}


// Called every frame
void UTraceComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsAttacking || !SkeletalComp) { return; }
	
	TArray<FHitResult> AllResults;

	// Perform Trace for each Socket Pair
	for (const FTraceSocketsRH Socket: Sockets)
	{
		// Get Socket Locations and Rotation
		FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(Socket.Start) };
		FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(Socket.End) };
		FQuat ShapeRotation { SkeletalComp->GetSocketQuaternion(Socket.Rotation) };
		
		TArray<FHitResult> OutResults;

		// Calculate the distance between the two sockets
		double WeaponDistance{ 
			FVector::Distance(StartSocketLocation, EndSocketLocation) };

		// Define the box half extent
		FVector BoxHalfExtent{ 
			BoxCollisonLength, BoxCollisonLength, WeaponDistance };
		
		BoxHalfExtent /= 2;
		
		// Perform the box trace
		FCollisionShape Box{ 
			FCollisionShape::MakeBox(BoxHalfExtent) };
		
		// Set up query parameters to ignore the owner
		FCollisionQueryParams IgnoreParams{ 
			FName { TEXT("Ignore Params") }, false, GetOwner() };
	
		// perform sweep multi by channel to get all hits
		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults, StartSocketLocation, EndSocketLocation, ShapeRotation,
			TraceChannel, Box, IgnoreParams) };
		
		for (FHitResult Hit : OutResults)
		{
			// Add each hit result to the AllResults array
			AllResults.Add(Hit);
		}

		// Debug Box to visualize the trace
		if (bDebugMode)
		{
			FVector CenterPoint{
				UKismetMathLibrary::VLerp(StartSocketLocation, EndSocketLocation, 0.5f) };

			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(), CenterPoint, Box.GetExtent(),
				bHasFoundTargets ? FColor::Green : FColor::Red,
				ShapeRotation.Rotator(), 1.0f, 2.0f);
		}
	}
	// If no hits, return
	if (AllResults.Num() == 0) { return; }

	// Get Damage from Combat Interface
	float CharacterDamage{ 0.0f };
	AActor* OwnerActor = GetOwner();
	ICombatRH* CombatInterface{ Cast<ICombatRH>(GetOwner()) };

	// If the cast is successful, get the damage value
	if (OwnerActor && OwnerActor->GetClass()->ImplementsInterface(UCombatRH::StaticClass()))
	{
		CharacterDamage = ICombatRH::Execute_GetDamage(OwnerActor, CurrentDamageType);
	}
	
	FDamageEvent TargetAttackEvent;

	// Apply damage to each hit actor, ignoring duplicates and already hit targets
	for (const FHitResult& Hit: AllResults)
	{
		AActor* TargetActor { Hit.GetActor() };

		if (TargetsToIgnore.Contains(TargetActor)) { continue; }

		TargetActor->TakeDamage(CharacterDamage,
			TargetAttackEvent, GetOwner()->GetInstigatorController(), GetOwner());

		TargetsToIgnore.AddUnique(TargetActor);
	}
}

// Reset the list of targets to ignore
void UTraceComponentRH::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}

// Set the current damage type
void UTraceComponentRH::SetCurrentDamageType(EDamageTypesRH InType)
{
	CurrentDamageType = InType;
}

