// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EDamageTypesRH
{
	None UMETA(DisplayName = "None Selected"),
	LightAttack UMETA(DisplayName = "Light Attack"),
	HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	GunShot UMETA(DisplayName = "Gun Shot"),
	ChargeShot UMETA(DisplayName = "Charge Shot"),
};
