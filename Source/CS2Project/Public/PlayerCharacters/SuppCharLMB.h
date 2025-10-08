// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SuppCharLMB.generated.h"

UCLASS()
class CS2PROJECT_API ASuppCharLMB : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASuppCharLMB();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
class USuppCharCompLMB* SuppCharLMB;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
