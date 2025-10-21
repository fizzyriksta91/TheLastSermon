// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/ProjectileBaseRH.h"

#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectileBaseRH::AProjectileBaseRH()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(8.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBaseRH::OnHit);
	RootComponent = CollisionComp;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.0f;
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


void AProjectileBaseRH::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		// check if both owner and hit actor are player controlled
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		APawn* HitPawn = Cast<APawn>(OtherActor);

		if (OwnerPawn && HitPawn)
		{
			bool bOwnerIsPlayer = OwnerPawn->IsPlayerControlled();
			bool bHitIsPlayer = HitPawn->IsPlayerControlled();

			// don't do damage to players
			if (bOwnerIsPlayer && bHitIsPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("Blocked friendly fire from %s to %s"),
					   *GetOwner()->GetName(), *OtherActor->GetName());
				Destroy();
				return;
			}
		}

		// Create a custom damage event to pass damage type info
		FDamageEvent DamageEvent;
        
		// Apply damage - the target's TakeDamage will use this
		float ActualDamage = OtherActor->TakeDamage(
			Damage, 
			DamageEvent, 
			GetInstigatorController(), 
			this);

		UE_LOG(LogTemp, Warning, TEXT("Projectile hit %s, Applied Damage: %f, DamageType: %d"),
			   *OtherActor->GetName(), ActualDamage, DamageType);

		Destroy();
	}
}

