// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/RangePlayerCharacterRH.h"

#include "PlayerCharacters/Components/LanternDamageComponentRH.h"
#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "PlayerCharacters/Components/RangeCombatComponentRH.h"

ARangePlayerCharacterRH::ARangePlayerCharacterRH()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangeCombatComp = CreateDefaultSubobject<URangeCombatComponentRH>(TEXT("Range Combat Component"));
	LanternDamageComp = CreateDefaultSubobject<ULanternDamageComponentRH>(TEXT("Lantern Damage Component"));
	LockOnComp = CreateDefaultSubobject<ULockOnComponentRH>(TEXT("Lock On Component"));
}

void ARangePlayerCharacterRH::BeginPlay()
{
	Super::BeginPlay();

	LanternDamageComp = FindComponentByClass<ULanternDamageComponentRH>();
	if (!LanternDamageComp)
	{
		UE_LOG(LogTemp, Warning, TEXT(
			"Ranged player: LanternDamageComponentRH not found on %s"), *GetName());
	}

	if (LanternBlueprintClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedLantern = GetWorld()->SpawnActor<AActor>(LanternBlueprintClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		if (SpawnedLantern)
		{
			// Attach to mesh socket if provided, otherwise attach to root
			USceneComponent* AttachTarget = GetRootComponent();
			if (LanternAttachSocket != NAME_None && GetMesh())
			{
				AttachTarget = GetMesh();
			}

			SpawnedLantern->AttachToComponent(AttachTarget, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LanternAttachSocket);
			UE_LOG(LogTemp, Warning, TEXT("Spawned lantern %s for %s"), *SpawnedLantern->GetName(), *GetName());

			// Register the lantern with the damage component so player is considered "in light"
			if (LanternDamageComp)
			{
				LanternDamageComp->EnterLight(SpawnedLantern);
				UE_LOG(LogTemp, Warning, TEXT("Registered spawned lantern with LanternDamageComponent"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn lantern BP for %s"), *GetName());
		}
	}
}
