// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SharedCameraManager.generated.h"

class ASharedCamera;

UCLASS()
class CS2PROJECT_API ASharedCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<ASharedCamera> SharedCameraClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	ASharedCamera* CameraRef;
};
