// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class CS2PROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AEnemyBaseCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 EnemiesToSpawn { 10 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnRadius { 500.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bAutoSpawnOnBeginPlay { true };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void SpawnEnemies();

	FVector GetRandomSpawnLocation() const;

public:

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnAllEnemies();

	UFUNCTION()
	void OnEnemyDeath(ABaseCharacter* DeadEnemy);

};
