// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "PlayerCharacters/Interfaces/EnemyRH.h"
#include "EnemyBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API AEnemyBaseCharacter : public ABaseCharacter , public IEnemyRH
{
	GENERATED_BODY()

	
public:
	AEnemyBaseCharacter();

	virtual float SetMovementSpeed(EMovementSpeedRH SpeedType) override;

protected:
	virtual void BeginPlay() override;
	
};
