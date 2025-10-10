// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "PlayerCharacters/Enums/EEnemyStatesRH.h"
#include "EnemyBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API AEnemyBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	
public:
	AEnemyBaseCharacter();

protected:
	virtual void BeginPlay() override;
	
};
