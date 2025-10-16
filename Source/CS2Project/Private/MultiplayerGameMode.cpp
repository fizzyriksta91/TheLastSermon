// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "EngineUtils.h"
#include "SharedCamera.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

// Adds a local player to the game
void AMultiplayerGameMode::AddLocalPlayer()
{
	// Ensure the engine and world are valid
	if (GEngine && GetWorld())
	{
		// Get the game instance
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();

		// Ensure the game instance is valid
		if (GameInstance)
		{
			FString Error;

			// Create a new local player
			ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(-1, Error, true);

			// Check if the player was created successfully
			if (NewPlayer)
			{
				UE_LOG(LogTemp, Display, TEXT("Local player added"));

				//Find all player controllers
				TArray<APlayerController*> PlayerControllers;
				for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					// Add each player controller to the array
					PlayerControllers.Add(Iterator->Get());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to add local player"));
			}
		}
	}	
}
