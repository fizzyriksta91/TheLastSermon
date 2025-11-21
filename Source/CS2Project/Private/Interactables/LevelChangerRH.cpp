// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/LevelChangerRH.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelChangerRH::ALevelChangerRH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(StaticMesh);
	InteractionSphere->SetSphereRadius(200.f);

}

void ALevelChangerRH::Interact_Implementation(ABaseCharacter* InteractingActor)
{
	IInteractableRH::Interact_Implementation(InteractingActor);

	if (!InteractingActor)
		return;

	if (LevelName.IsNone())
		return;

	UGameplayStatics::OpenLevel(this, LevelName);
}

// Called when the game starts or when spawned
void ALevelChangerRH::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelChangerRH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

