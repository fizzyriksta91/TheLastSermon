// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeComponentRH.h"

#include "GameFramework/Character.h"

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

void UDodgeComponentRH::Dodge()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && !bIsDodging)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastDodgeTime < DodgeCooldown)
		{
			return;
		}

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
			return;
		
		bIsDodging = true;

		AnimInstance->Montage_Play(DodgeMontage);
		UE_LOG(LogTemp, Warning, TEXT("Dodge Complete"));
		
			// You can add additional logic here, such as applying invincibility frames or movement adjustments

		LastDodgeTime = CurrentTime;
		bIsDodging = false;
	}
}

