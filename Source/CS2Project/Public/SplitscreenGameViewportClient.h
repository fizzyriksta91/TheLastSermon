// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "SplitscreenGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API USplitscreenGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	bool bDisableSplitscreenOverride = false;
	
	UFUNCTION(BlueprintCallable)
	void SetActiveSplitscreenType(ESplitScreenType::Type DynamicSplitscreen);

private:
	ESplitScreenType::Type CurrentSplitscreenType = ESplitScreenType::None;
};
