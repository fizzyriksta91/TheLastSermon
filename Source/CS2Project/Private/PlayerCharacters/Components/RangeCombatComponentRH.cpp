// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/RangeCombatComponentRH.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
URangeCombatComponentRH::URangeCombatComponentRH()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URangeCombatComponentRH::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
}


// Called every frame
void URangeCombatComponentRH::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URangeCombatComponentRH::PerformPrimaryRangedAttack()
{
	if (!ProjectileClass || !CharacterRef) { return; }

	FVector SpawnLocation = CharacterRef->GetActorLocation() +
		CharacterRef->GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = CharacterRef->GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = CharacterRef->GetInstigator();

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
}

