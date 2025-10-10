// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyStatesRH
{
	Idle UMETA(DisplayName = "Idle"),
	Chasing UMETA(DisplayName = "Chasing"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dead UMETA(DisplayName = "Dead")
};

