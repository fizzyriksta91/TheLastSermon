// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerCharacters/Components/DodgeComponentRH.h"

#include "GameFramework/Character.h"
#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"
#include "PlayerCharacters/Components/RangeCombatComponentRH.h"

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
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	// If no character or already dodging, return
	if (!Character || bIsDodging) { return; }

	float CurrentTime = GetWorld()->GetTimeSeconds();
    
	// Check if the dodge is off cooldown
	if (CurrentTime - LastDodgeTime < DodgeCooldown)
	{
		return;
	}

	// Check melee combat component
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();
    
	// Check ranged combat component
	URangeCombatComponentRH* RangeCombatComp = GetOwner()->FindComponentByClass<URangeCombatComponentRH>();

	// Can only dodge if not attacking
	bool bCanDodge = true;
	if (MeleeCombatComp)
	{
		bCanDodge = MeleeCombatComp->bCanAttack;
	}

	if (!bCanDodge) { return; }

	// Play dodge animation
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance || !DodgeMontage) { return; }

	// Start dodge and set timer
	float DodgeDuration = 0.5f;
	AnimInstance->Montage_Play(DodgeMontage);
	bIsDodging = true;
	LastDodgeTime = CurrentTime;

	// Disable combat during dodge
	if (MeleeCombatComp)
	{
		MeleeCombatComp->bCanAttack = false;
	}

	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle,
		this, &UDodgeComponentRH::OnDodgeFinished, DodgeDuration, false);
}

// Function called when the dodge action is finished
void UDodgeComponentRH::OnDodgeFinished()
{
	bIsDodging = false;
    
	// Re-enable combat
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();
	if (MeleeCombatComp)
	{
		MeleeCombatComp->bCanAttack = true;
	}
}

