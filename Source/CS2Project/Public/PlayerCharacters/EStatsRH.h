// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EStatsRH
{
	None UMETA(DisplayName = "None Selected"),
	Health UMETA(DisplayName = "Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Strength UMETA(DisplayName = "Strength"),
	Cooldown UMETA(DisplayName = "Cooldown"),
	MaxCooldown UMETA(DisplayName = "Max Cooldown"),
	MeleeRange UMETA(DisplayName = "MeleeRange")
};
