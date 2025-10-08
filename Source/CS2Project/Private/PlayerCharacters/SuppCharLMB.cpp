// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/SuppCharLMB.h"

#include "PlayerCharacters/Components/SuppCharCompLMB.h"


// Sets default values
ASuppCharLMB::ASuppCharLMB()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SuppCharLMB = CreateDefaultSubobject<USuppCharCompLMB>(TEXT("Support Character Component"));
}

// Called when the game starts or when spawned
void ASuppCharLMB::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASuppCharLMB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASuppCharLMB::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

