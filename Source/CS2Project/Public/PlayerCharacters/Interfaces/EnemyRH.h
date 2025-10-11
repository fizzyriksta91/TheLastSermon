// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacters/Enums/EMovementSpeedRH.h"
#include "UObject/Interface.h"
#include "EnemyRH.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyRH : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CS2PROJECT_API IEnemyRH
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float SetMovementSpeed(EMovementSpeedRH SpeedType) = 0;
};
