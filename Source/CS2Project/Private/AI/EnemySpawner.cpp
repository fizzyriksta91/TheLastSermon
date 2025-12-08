// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"

#include "AI/EnemyAIControllerRH.h"
#include "AI/EnemyBaseCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoSpawnOnBeginPlay)
	{
		SpawnAllEnemies();
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Spawns enemies at a random location within the spawn radius
void AEnemySpawner::SpawnEnemies()
{
	if (!EnemyClass)
		return;

	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	const float TraceUp = 500.f;
	const float TraceDown = 1000.f;
	const float GroundOffset =2.f;
	
	FVector TraceStart = SpawnLocation + FVector(0.f, 0.f, TraceUp);
	FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, TraceDown);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	// Perform line trace to find ground level
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	if (bHit)
	{
		SpawnLocation.Z = Hit.Location.Z + GroundOffset;
	}
	
	// Set spawn parameters to handle collision
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AEnemyBaseCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBaseCharacter>(
		EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (!SpawnedEnemy)
		return;
	
	// Adjust Z location based on capsule half-height to prevent sinking into ground
	UCapsuleComponent* Capsule = SpawnedEnemy->FindComponentByClass<UCapsuleComponent>();
	if (Capsule && bHit)
	{
		FVector NewLocation = SpawnedEnemy->GetActorLocation();
		NewLocation.Z = Hit.Location.Z + Capsule->GetScaledCapsuleHalfHeight();
		SpawnedEnemy->SetActorLocation(
			NewLocation, false, nullptr,
			ETeleportType::TeleportPhysics);
	}

	SpawnedEnemy->SpawnDefaultController();
	AEnemyAIControllerRH* AICont = Cast<AEnemyAIControllerRH>(SpawnedEnemy->GetController());
	
	// If no AI controller was spawned, create one manually
	if (!AICont)
	{
		FActorSpawnParameters AIParams;
		AIParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEnemyAIControllerRH* NewAI = GetWorld()->SpawnActor<AEnemyAIControllerRH>(
				AEnemyAIControllerRH::StaticClass(), SpawnLocation, SpawnRotation, AIParams);
		if (NewAI)
		{
			NewAI->Possess(SpawnedEnemy);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(
				"Failed to spawn AI controller for %s"), *SpawnedEnemy->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultController provided controller %s for %s"),
		 *AICont->GetName(), *SpawnedEnemy->GetName());
	}
	
	SpawnedEnemy->OnCharacterDeath.AddDynamic(this, &AEnemySpawner::OnEnemyDeath);
}

// Generates a random spawn location within the defined spawn radius
FVector AEnemySpawner::GetRandomSpawnLocation() const
{
	FVector Origin = GetActorLocation();
	float RandomAngle = FMath::FRandRange(0.f, 360.f);
	float RandomRadius = FMath::FRandRange(0.f, SpawnRadius);

	float X = Origin.X + (RandomRadius * FMath::Cos(FMath::DegreesToRadians(RandomAngle)));
	float Y = Origin.Y + (RandomRadius * FMath::Sin(FMath::DegreesToRadians(RandomAngle)));

	return FVector(X, Y, Origin.Z);
}

// Spawns the specified number of enemies at level start
void AEnemySpawner::SpawnAllEnemies()
{
	if (!EnemyClass)
		return;

	for (int32 i = 0; i < EnemiesToSpawn; ++i)
	{
		SpawnEnemies();
	}
}

void AEnemySpawner::OnEnemyDeath(ABaseCharacter* DeadEnemy)
{
	// Guard against null or pending-kill pointers to avoid access violations
	if (!IsValid(DeadEnemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnEnemyDeath: DeadEnemy is null or pending kill (from %s)"),
			*GetName());
		return;
	}
}

