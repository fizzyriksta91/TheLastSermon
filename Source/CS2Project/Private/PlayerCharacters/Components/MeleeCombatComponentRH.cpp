// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UMeleeCombatComponentRH::UMeleeCombatComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeCombatComponentRH::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeleeCombatComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeCombatComponentRH::PerformLightComboAttack()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && !bIsAttacking && LightComboMontages.Num() > 0)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
			return;
		
		bIsAttacking = true;
		for (UAnimMontage* Montage : LightComboMontages)
		{
			if (Montage == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("LightComboMontages contains a null montage!"));
				return;
			}
		}
		// Play the first montage in the array
		AnimInstance->Montage_Play(LightComboMontages[0]);
		UE_LOG(LogTemp, Warning, TEXT("Light Combo Attack Performed"));

		// You can add additional logic here, such as applying damage or effects

		bIsAttacking = false;
	}
}

