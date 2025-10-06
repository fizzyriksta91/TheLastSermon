// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FTraceSocketsRH.generated.h"

USTRUCT(BlueprintType)
struct CS2PROJECT_API FTraceSocketsRH
{

	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FName Start;

	UPROPERTY(EditAnywhere)
	FName End;

	UPROPERTY(EditAnywhere)
	FName Rotation;
};
