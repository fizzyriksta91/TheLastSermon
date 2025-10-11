// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EEnemyStatesRH
{
	IdleState UMETA(DisplayName = "Idle"),
	ChasingState UMETA(DisplayName = "Chasing"),
	AttackingState UMETA(DisplayName = "Attacking"),
	DeadState UMETA(DisplayName = "Dead")
};

