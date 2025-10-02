// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplitScreenManager.generated.h"

UCLASS()
class CS2PROJECT_API ASplitScreenManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<APlayerController*> Players;

	UPROPERTY()
	float SplitDistance{ 500.0f };
	
public:	
	// Sets default values for this actor's properties
	ASplitScreenManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayers(const TArray<APlayerController*>& InPlayers);
	

};
