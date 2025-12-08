// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/LanternDamageComponentRH.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
ULanternDamageComponentRH::ULanternDamageComponentRH()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULanternDamageComponentRH::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = Cast<ACharacter>(GetOwner());

	if (CharacterRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("LanternDamageComp initialized for: %s"), 
			*CharacterRef->GetName());
	}

	// Start the timer to apply darkness damage at regular intervals
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle, this, &ULanternDamageComponentRH::ApplyDarknessDamage,
		DamageTickrate, true);
	
}


// Called every frame
void ULanternDamageComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Enter light source
void ULanternDamageComponentRH::EnterLight(AActor* Lantern)
{
	if (!Lantern || ActiveLanterns.Contains(Lantern)) 
		return;

	ActiveLanterns.AddUnique(Lantern);
	UpdateLightStatus();
}

// Exit light source
void ULanternDamageComponentRH::ExitLight(AActor* Lantern)
{
	if (!Lantern) { return; }

	ActiveLanterns.Remove(Lantern);
	UpdateLightStatus();
	
}

// Apply damage when in darkness 
void ULanternDamageComponentRH::ApplyDarknessDamage()
{
	if (bIsInLight || !CharacterRef) { return; }

	FDamageEvent DamgeEvent;
	CharacterRef->TakeDamage(DarknessTickDamage,
		DamgeEvent, nullptr, nullptr);

	UE_LOG(LogTemp, Error, TEXT("CHARACTER %s is taking %f darkness damage!"), 
		*CharacterRef->GetName(), DarknessTickDamage);
}

// Update status of character within light or darkness
void ULanternDamageComponentRH::UpdateLightStatus()
{
	bIsInLight = ActiveLanterns.Num() > 0;

	if (bIsInLight)
	{
		UE_LOG(LogTemp, Display, TEXT("Character is in light"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Character is in darkness"));
	}
}

