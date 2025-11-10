// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AOEBaseRH.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

AAOEBaseRH::AAOEBaseRH()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default object type = Pawn
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void AAOEBaseRH::BeginPlay()
{
	Super::BeginPlay();
	
	if (bTriggerOnBeginPlay)
	{
		if (DelayOnBeginPlay <= 0.f)
		{
			Trigger();
		}
		else
		{
			GetWorldTimerManager().SetTimer(TriggerBeginPlayHandle, this, &AAOEBaseRH::Trigger, DelayOnBeginPlay, false);
		}
	}
}

void AAOEBaseRH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAOEBaseRH::SpawnAOESphere()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpherePos = GetActorLocation();

	// Optional debug draw (matches Blueprint Draw Debug Sphere node)
	if (bDrawDebugSphere)
	{
		// Params: World, Center, Radius, Segments, Color, Persistent, LifeTime, DepthPriority, Thickness
		DrawDebugSphere(World, SpherePos, Radius, 12, FColor::Red, false, 1.0f, 0, 1.0f);
	}

	// Prepare ActorsToIgnore (optionally include instigator)
	TArray<AActor*> ActorsToIgnore;
	if (bIgnoreInstigator)
	{
		APawn* Inst = GetInstigator();
		if (Inst)
		{
			ActorsToIgnore.Add(Inst);
		}
	}

	// Perform the sphere overlap (fills OutActors)
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(
		World,
		SpherePos,
		Radius,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);

	// For each overlapped actor, call the handler
	for (AActor* HitActor : OutActors)
	{
		if (HitActor && HitActor != this)
		{
			OnAOEOverlapActor(HitActor);
		}
	}
}

void AAOEBaseRH::OnAOEOverlapActor_Implementation(AActor* OtherActor)
{
	// Default: do nothing. Blueprint can override this.
}

void AAOEBaseRH::Trigger()
{
	// Mirror the Blueprint custom event: spawn the AOE sphere logic
	SpawnAOESphere();
}