// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableRH.h"
#include "LevelChangerRH.generated.h"

UCLASS()
class CS2PROJECT_API ALevelChangerRH : public AActor, public IInteractableRH
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChangerRH();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName LevelName;

	virtual void Interact_Implementation(AActor* InteractingActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* InteractionSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
