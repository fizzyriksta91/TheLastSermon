// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CampFireRH.h"

#include "Components/SphereComponent.h"
#include "PlayerCharacters/BaseCharacter.h"

// Sets default values
ACampFireRH::ACampFireRH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(StaticMesh);
	InteractionSphere->SetSphereRadius(100.f);
}

// Called when the game starts or when spawned
void ACampFireRH::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACampFireRH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Heal the interacting actor to full health
void ACampFireRH::Interact_Implementation(ABaseCharacter* InteractingActor)
{
	if (!InteractingActor)
		return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(InteractingActor);
	if (Character)
	{
		Character->HealToFull();
	}
}
