// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UStatsComponentRH::UStatsComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponentRH::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatsComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Reduce Health by Amount and clamp to 0 and MaxHealth
void UStatsComponentRH::ReduceHealth(float Amount)
{
	if (Stats[EStatsRH::Health] <= 0.0f) { return; }

	Stats[EStatsRH::Health] -= Amount;

	Stats[EStatsRH::Health] = UKismetMathLibrary::FClamp(
		Stats[EStatsRH::Health], 0, Stats[EStatsRH::MaxHealth]);
}

void UStatsComponentRH::EnableRegen()
{
	bCanRegen = true;
}

