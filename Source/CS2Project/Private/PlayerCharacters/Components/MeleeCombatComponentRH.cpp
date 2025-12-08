// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"

// Sets default values for this component's properties
UMeleeCombatComponentRH::UMeleeCombatComponentRH()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMeleeCombatComponentRH::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef =  GetOwner<ACharacter>();
	LastHeavyAttackTime = -HeavyAttackCooldown;
	
	// Bind to Trace Component's OnHit event
	if (CharacterRef)
	{
		if (UTraceComponentRH* TraceComp = CharacterRef->FindComponentByClass<UTraceComponentRH>())
		{
			TraceComp->OnHit.AddDynamic(this, &UMeleeCombatComponentRH::HandleTraceHit);
		}
	}
}


// Called every frame
void UMeleeCombatComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bMovementLocked && CharacterRef)
	{
		if (USkeletalMeshComponent* Mesh = CharacterRef->GetMesh())
		{
			if (UAnimInstance* AnimInst = Mesh->GetAnimInstance())
			{
				// If there is no current montage or it's not playing anymore, unlock.
				bool bIsPlaying = CurrentAttackMontage && AnimInst->Montage_IsPlaying(CurrentAttackMontage);
				if (!bIsPlaying)
				{
					UE_LOG(LogTemp, Warning, TEXT("MeleeCombat: safety unlock - montage not playing"));
					UnlockMovement();
				}
			}
			else
			{
				// No anim instance — ensure we don't stay locked
				UE_LOG(LogTemp, Warning, TEXT("MeleeCombat: safety unlock - no AnimInstance"));
				UnlockMovement();
			}
		}
	}
}

void UMeleeCombatComponentRH::LockMovement()
{
	if (!CharacterRef || bMovementLocked) 
		return;
	
	if (UCharacterMovementComponent* MoveComp = CharacterRef->GetCharacterMovement())
	{
		MoveComp->DisableMovement();
		bMovementLocked = true;
		UE_LOG(LogTemp, Log, TEXT("MeleeCombat: movement locked"));
	}
}

void UMeleeCombatComponentRH::UnlockMovement()
{
	if (!CharacterRef || !bMovementLocked) 
		return;
	
	if (UCharacterMovementComponent* MoveComp = CharacterRef->GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
		bMovementLocked = false;
		UE_LOG(LogTemp, Log, TEXT("MeleeCombat: movement unlocked"));
	}
}

void UMeleeCombatComponentRH::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("MeleeCombat: OnAttackMontageEnded called (Interrupted=%s)"),
		bInterrupted ? TEXT("true") : TEXT("false"));
	
	if (Montage && Montage == CurrentAttackMontage)
	{
		UnlockMovement();
		CurrentAttackMontage = nullptr;
	}
}

// Light Combo Attack
void UMeleeCombatComponentRH::PerformLightComboAttack()
{
	if (!bCanAttack) 
		return; 

	RotateTowardsNearestEnemy();

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
	
	// Play Combo Montage and manage combo counter
	bCanAttack = false;
	
	if (LightComboMontages.Num() == 0) 
		return;
	int SafeIndex = ComboCounter;
	if (SafeIndex < 0 || SafeIndex >= LightComboMontages.Num()) SafeIndex = 0;

	UAnimMontage* MontageToPlay = LightComboMontages[SafeIndex];
	if (MontageToPlay && CharacterRef)
	{
		if (UAnimInstance* AnimInst = CharacterRef->GetMesh()->GetAnimInstance())
		{
			CurrentAttackMontage = MontageToPlay;

			MontageEndDelegate.BindUObject(this, &UMeleeCombatComponentRH::OnAttackMontageEnded);
			AnimInst->Montage_SetEndDelegate(MontageEndDelegate, MontageToPlay);

			LockMovement();
		}
		CharacterRef->PlayAnimMontage(MontageToPlay);
	}

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

// Reset attack ability
void UMeleeCombatComponentRH::ResetAttack()
{
	bCanAttack = true;
}

// Reset combo counter
void UMeleeCombatComponentRH::ResetCombatCounter()
{
	ComboCounter = 0;
}

// perform Heavy Attack
void UMeleeCombatComponentRH::PerformHeavyAttack()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHeavyAttackTime < HeavyAttackCooldown) 
		return; 

	if (!bCanAttack) 
		return; 

	RotateTowardsNearestEnemy();

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
	
	bCanAttack = false;
	LastHeavyAttackTime = CurrentTime;
	if (HeavyAttackMontage && CharacterRef)
	{
		if (UAnimInstance* AnimInst = CharacterRef->GetMesh()->GetAnimInstance())
		{
			CurrentAttackMontage = HeavyAttackMontage;

			MontageEndDelegate.BindUObject(this, &UMeleeCombatComponentRH::OnAttackMontageEnded);
			AnimInst->Montage_SetEndDelegate(MontageEndDelegate, HeavyAttackMontage);

			LockMovement();
		}
		CharacterRef->PlayAnimMontage(HeavyAttackMontage);
	}
}

// Handle hit event from Trace Component
void UMeleeCombatComponentRH::HandleTraceHit(AActor* HitActor, FVector HitLocation)
{
	if (!HitActor || HitActor == GetOwner()) 
		return;

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitLocation);
	}
}

// Rotate character towards nearest enemy when attacking
void UMeleeCombatComponentRH::RotateTowardsNearestEnemy()
{
	// Find and lock onto nearest enemy when attacking
	if (auto LockOnComp = CharacterRef->FindComponentByClass<ULockOnComponentRH>())
	{
		TArray<AActor*> NearbyEnemies = LockOnComp->FindEnemiesInRadius(1000.0f);
		if (NearbyEnemies.Num() > 0)
		{
			AActor* ClosestEnemy = LockOnComp->FindClosestEnemy(NearbyEnemies);
			if (ClosestEnemy)
			{
				LockOnComp->RotateTowardsTarget(ClosestEnemy, GetWorld()->GetDeltaSeconds());
			}
		}
	}
}



