// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "EngineUtils.h"
#include "SplitScreenManager.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"


void AMultiplayerGameMode::AddLocalPlayer()
{
	if (GEngine && GetWorld())
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{

			FString Error;
			ULocalPlayer* NewPlayer = GameInstance->CreateLocalPlayer(-1, Error, true);
			if (NewPlayer)
			{
				UE_LOG(LogTemp, Display, TEXT("Local player added"));

				//Find all player controllers
				TArray<APlayerController*> PlayerControllers;
				for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
				{
					PlayerControllers.Add(Iterator->Get());
				}
				//Find or spawn the SplitScreenManager
				for (TActorIterator<ASplitScreenManager> It(GetWorld()); It; ++It)
				{
					ASplitScreenManager* SplitScreenManager= *It;
					if (SplitScreenManager)
					{
						SplitScreenManager->SetPlayers(PlayerControllers);
						break;
					}
					
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to add local player"));
				UE_LOG(LogTemp, Warning, TEXT("ViewportClient: %s"), *GetWorld()->GetGameViewport()->GetClass()->GetName());
			}
		}
	}	
}
