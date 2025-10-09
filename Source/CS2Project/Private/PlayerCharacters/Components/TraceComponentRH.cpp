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

	if (!bIsAttacking) { return; }

	TArray<FHitResult> AllResults;

	for (const FTraceSocketsRH Socket: Sockets)
	{
		FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(Socket.Start) };
		FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(Socket.End) };
		FQuat ShapeRotation { SkeletalComp->GetSocketQuaternion(Socket.Rotation) };

		TArray<FHitResult> OutResults;
		double WeaponDistance{ 
			FVector::Distance(StartSocketLocation, EndSocketLocation) };
		
		FVector BoxHalfExtent{ 
			BoxCollisonLength, BoxCollisonLength, WeaponDistance };
		
		BoxHalfExtent /= 2;
		
		FCollisionShape Box{ 
			FCollisionShape::MakeBox(BoxHalfExtent) };
		
		FCollisionQueryParams IgnoreParams{ 
			FName { TEXT("Ignore Params") }, false, GetOwner() };
	
		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults, StartSocketLocation, EndSocketLocation, ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1, Box, IgnoreParams) };

		for (FHitResult Hit : OutResults)
		{
			AllResults.Add(Hit);
		}

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

	if (AllResults.Num() == 0) { return; }

	float CharacterDamage{ 0.0f };

	ICombatRH* CombatInterface{ Cast<ICombatRH>(GetOwner()) };
	if (CombatInterface)
	{
		CharacterDamage = CombatInterface->GetDamage(CurrentDamageType);
	}

	FDamageEvent TargetAttackEvent;

	for (const FHitResult& Hit: AllResults)
	{
		AActor* TargetActor { Hit.GetActor() };

		if (TargetsToIgnore.Contains(TargetActor)) { continue; }

		TargetActor->TakeDamage(CharacterDamage,
			TargetAttackEvent, GetOwner()->GetInstigatorController(), GetOwner());

		TargetsToIgnore.AddUnique(TargetActor);
	}
}

void UTraceComponentRH::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}

void UTraceComponentRH::SetCurrentDamageType(EDamageTypesRH InType)
{
	CurrentDamageType = InType;
}

