// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerCharacters/Components/DodgeComponentRH.h"

#include "GameFramework/Character.h"
#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

// Sets default values for this component's properties
UDodgeComponentRH::UDodgeComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDodgeComponentRH::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDodgeComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Function to initiate the dodge action
void UDodgeComponentRH::Dodge()
{
	// Ensure the owner is a character and can dodge
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();

	// Check if the character can attack (not in the middle of an attack)
	if (MeleeCombatComp)
	{
		bool bCanAttack = MeleeCombatComp->bCanAttack;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	// Check if the character is valid, not already dodging, and can attack
	if (Character && !bIsDodging && MeleeCombatComp && MeleeCombatComp->bCanAttack)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		
		// Check if the dodge is off cooldown
		if (CurrentTime - LastDodgeTime < DodgeCooldown)
		{
			return;
		}

		// Play the dodge animation montage if assigned
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
			return;

		// Start the dodge action and set the timer to end it
		float DodgeDuration = 0.5f; 
		AnimInstance->Montage_Play(DodgeMontage);
		bIsDodging = true;
        MeleeCombatComp->bCanAttack = false;
		LastDodgeTime = CurrentTime;
		
		// You can add additional logic here, such as applying invincibility frames or movement adjustments

		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle,
			this, &UDodgeComponentRH::OnDodgeFinished, DodgeDuration, false);
	}
}

// Function called when the dodge action is finished
void UDodgeComponentRH::OnDodgeFinished()
{
	bIsDodging = false;
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();
	if (MeleeCombatComp)
	{
		MeleeCombatComp->bCanAttack = true;
	}
}

