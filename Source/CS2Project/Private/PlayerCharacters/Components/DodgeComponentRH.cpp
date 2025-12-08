// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerCharacters/Components/DodgeComponentRH.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"
#include "PlayerCharacters/Components/RangeCombatComponentRH.h"

// Sets default values for this component's properties
UDodgeComponentRH::UDodgeComponentRH()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDodgeComponentRH::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UDodgeComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Function to initiate the dodge action
void UDodgeComponentRH::Dodge()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	
	if (!Character || bIsDodging) 
		return;

	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastDodgeTime < DodgeCooldown)
		return;
	
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();
	URangeCombatComponentRH* RangeCombatComp = GetOwner()->FindComponentByClass<URangeCombatComponentRH>();

	// Can only dodge if not attacking
	bool bCanDodge = true;
	if (MeleeCombatComp)
	{
		bCanDodge = MeleeCombatComp->bCanAttack;
	}

	if (RangeCombatComp && RangeCombatComp->IsCharging())
	{
		bCanDodge = false;
	}

	if (!bCanDodge) 
		return;
	
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance || !DodgeMontage) 
		return; 

	// Start dodge and set timer
	float DodgeDuration = 0.5f;
	AnimInstance->Montage_Play(DodgeMontage);
	bIsDodging = true;
	bIsInvulnerable = true;
	LastDodgeTime = CurrentTime;
	
	if (DodgeSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DodgeSound, Character->GetActorLocation());
	}

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
	bIsInvulnerable = false;
    
	// Re-enable combat
	UMeleeCombatComponentRH* MeleeCombatComp = GetOwner()->FindComponentByClass<UMeleeCombatComponentRH>();
	if (MeleeCombatComp)
	{
		MeleeCombatComp->bCanAttack = true;
	}
}

