// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
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
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to add local player"));
			}
		}
		
		
	}	
}
