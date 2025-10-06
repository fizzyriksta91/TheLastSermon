// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerRH.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerRH : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CS2PROJECT_API IPlayerRH
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool HasEnoughStamina(float Cost) { return true; }
};
