// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedCameraManager.h"

#include "SharedCamera.h"
#include "Kismet/GameplayStatics.h"

void ASharedCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Spawn the SharedCamera actor
	FVector SpawnLocation = FVector(0.f, 0.f, 0.f); 
	FRotator SpawnRotation = FRotator(-60.f, 0.f, 0.f); 
    
	ASharedCamera* SharedCam = GetWorld()->SpawnActor<ASharedCamera>(
		ASharedCamera::StaticClass(), SpawnLocation, SpawnRotation);

	if (SharedCam)
	{
		// Set as view target for both players
		APlayerController* PC0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APlayerController* PC1 = UGameplayStatics::GetPlayerController(GetWorld(), 1);

		if (PC0)
		{
			PC0->SetViewTargetWithBlend(SharedCam, 0.5f);
		}
		if (PC1)
		{
			PC1->SetViewTargetWithBlend(SharedCam, 0.5f);
		}
	}
}
