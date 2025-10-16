// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"
#include "PlayerCharacters/Interfaces/PlayerRH.h"

// Sets default values for this component's properties
UMeleeCombatComponentRH::UMeleeCombatComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMeleeCombatComponentRH::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef =  GetOwner<ACharacter>();

	LastHeavyAttackTime = -HeavyAttackCooldown;
}


// Called every frame
void UMeleeCombatComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Light Combo Attack
void UMeleeCombatComponentRH::PerformLightComboAttack()
{
	if (!bCanAttack) { return; }

	CurrentDamageType = EDamageTypesRH::LightAttack;

	// Set Damage Type in Trace Component
	if (CharacterRef)
	{
		auto TraceComp = CharacterRef->FindComponentByClass<UTraceComponentRH>();
		if (TraceComp)
		{
			TraceComp->SetCurrentDamageType(CurrentDamageType);
			TraceComp->HandleResetAttack();
		}
	}

	// Play Combo Montage
	bCanAttack = false;
	CharacterRef->PlayAnimMontage(LightComboMontages[ComboCounter]);
	ComboCounter++;
	int MaxCombo{ LightComboMontages.Num() };
	
	ComboCounter = UKismetMathLibrary::Wrap(
		ComboCounter, -1, (MaxCombo -1));

	//Start/Reset Timer to reset combo counter
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		ComboResetTimerHandle, this, &UMeleeCombatComponentRH::ResetCombatCounter,
		ComboResetDelay, false);
}

void UMeleeCombatComponentRH::ResetAttack()
{
	bCanAttack = true;
}

void UMeleeCombatComponentRH::ResetCombatCounter()
{
	ComboCounter = 0;
}

// perform Heavy Attack
void UMeleeCombatComponentRH::PerformHeavyAttack()
{
	// Check Cooldown
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHeavyAttackTime < HeavyAttackCooldown) { return; }

	if (!bCanAttack) { return; }

	// Set Damage Type in Trace Component
	CurrentDamageType = EDamageTypesRH::HeavyAttack;

	// Set Damage Type in Trace Component
	if (CharacterRef)
	{
		auto TraceComp = CharacterRef->FindComponentByClass<UTraceComponentRH>();
		if (TraceComp)
		{
			TraceComp->SetCurrentDamageType(CurrentDamageType);
			TraceComp->HandleResetAttack();
		}
	}

	// Play Heavy Attack Montage
	bCanAttack = false;
	LastHeavyAttackTime = CurrentTime;
	CharacterRef->PlayAnimMontage(HeavyAttackMontage);
}



