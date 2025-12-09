// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CameraPerspectiveTriggerRH.generated.h"

UCLASS()
class CS2PROJECT_API ACameraPerspectiveTriggerRH : public AActor
{
	GENERATED_BODY()

public:
	ACameraPerspectiveTriggerRH();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Perspective index to set on overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	int32 PerspectiveIndex = 0;

	// Optional: require the overlapping actor to be a player pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bRequirePlayerPawn = true;
};
