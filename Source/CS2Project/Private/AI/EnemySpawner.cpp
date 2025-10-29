// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemySpawner.h"

#include "AI/EnemyBaseCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

void AEnemySpawner::SpawnEnemies()
{
	if (!EnemyClass)
		return;

	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AEnemyBaseCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBaseCharacter>(
		EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedEnemy)
	{
		SpawnedEnemy->OnCharacterDeath.AddDynamic(this, &AEnemySpawner::OnEnemyDeath);
	}
}

FVector AEnemySpawner::GetRandomSpawnLocation() const
{
	FVector Origin = GetActorLocation();
	float RandomAngle = FMath::FRandRange(0.f, 360.f);
	float RandomRadius = FMath::FRandRange(0.f, SpawnRadius);

	float X = Origin.X + (RandomRadius * FMath::Cos(FMath::DegreesToRadians(RandomAngle)));
	float Y = Origin.Y + (RandomRadius * FMath::Sin(FMath::DegreesToRadians(RandomAngle)));

	return FVector(X, Y, Origin.Z);
}

void AEnemySpawner::SpawnAllEnemies()
{
	if (!EnemyClass)
		return;

	for (int32 i = 0; i < EnemiesToSpawn; ++i)
	{
		SpawnEnemies();
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawned %d enemies at level start"), EnemiesToSpawn);
}

void AEnemySpawner::OnEnemyDeath(ABaseCharacter* DeadEnemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy died: %s"), *DeadEnemy->GetName());
}

