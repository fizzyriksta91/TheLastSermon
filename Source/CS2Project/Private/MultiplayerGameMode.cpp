// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "EngineUtils.h"
#include "SharedCamera.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacters/BaseCharacter.h"

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
			// Log current state before adding
			UE_LOG(LogTemp, Warning, TEXT("=== BEFORE AddLocalPlayer ==="));
			UE_LOG(LogTemp, Warning, TEXT("Local Players: %d"), GameInstance->GetLocalPlayers().Num());
            
			int32 ControllerCount = 0;
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerController* PC = It->Get();
				UE_LOG(LogTemp, Warning, TEXT("Controller %d: %s | Pawn: %s"), 
					ControllerCount++, 
					*PC->GetName(), 
					PC->GetPawn() ? *PC->GetPawn()->GetName() : TEXT("NULL"));
			}

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

void AMultiplayerGameMode::NotifyCharacterDeath(ABaseCharacter* DeadCharacter)
{
	if (!HasAuthority() || !GetWorld())
		return;

	int32 AliveCount = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (!PlayerController)
			continue;

		APawn* Pawn = PlayerController->GetPawn();
		ABaseCharacter* Character = Cast<ABaseCharacter>(Pawn);
		if (Character && !Character->IsDead())
		{
			++AliveCount;
		}
	}

	if (AliveCount <= 0)
	{
		FString MapName = GetWorld()->GetMapName();
		FString ShortName = FPackageName::GetShortName(MapName);
		UGameplayStatics::OpenLevel(this, FName(*ShortName));
		UE_LOG(LogTemp, Warning, TEXT("All players are dead. Restarting level"));
	}
}

