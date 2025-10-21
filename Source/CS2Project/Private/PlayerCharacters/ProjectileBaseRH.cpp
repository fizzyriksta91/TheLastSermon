// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/ProjectileBaseRH.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectileBaseRH::AProjectileBaseRH()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(8.0f);
	RootComponent = CollisionComp;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AProjectileBaseRH::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBaseRH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

