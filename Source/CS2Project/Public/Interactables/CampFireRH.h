// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableRH.h"
#include "GameFramework/Actor.h"
#include "CampFireRH.generated.h"

UCLASS()
class CS2PROJECT_API ACampFireRH : public AActor , public IInteractableRH
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACampFireRH();

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

	virtual void Interact_Implementation(AActor* InteractingActor) override;

};
