// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/BaseCharacter.h"
#include "PlayerCharacters/Components/StatsComponentRH.h"
#include "PlayerCharacters/Components/TraceComponentRH.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponentRH>(TEXT("Stats Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponentRH>(TEXT("Trace Component"));

} 

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::FaceMovementDirection()
{
	// Get the character's velocity
	FVector Velocity = GetVelocity();
    
	// Only rotate if we're actually moving
	if (!Velocity.IsNearlyZero())
	{
		// Zero out Z component to keep rotation on XY plane
		Velocity.Z = 0.0f;
		Velocity.Normalize();
        
		// Convert to rotation and set actor rotation (only yaw component)
		FRotator NewRotation = Velocity.Rotation();
		LastMovementRotation = FRotator(0.f, NewRotation.Yaw, 0.f);
		SetActorRotation(LastMovementRotation);
		bHasMovedBefore = true;
	}
	else if (bHasMovedBefore)
	{
		// If not moving, maintain the last known movement direction
		SetActorRotation(LastMovementRotation);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Auto-face the movement direction
	FaceMovementDirection();

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

float ABaseCharacter::GetDamage()
{
	return StatsComp->Stats[EStatsRH::Strength];
}







