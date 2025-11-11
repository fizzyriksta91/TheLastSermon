// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

class ABaseCharacter;

UCLASS()
class CS2PROJECT_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void AddLocalPlayer();

	UFUNCTION()
	void NotifyCharacterDeath(ABaseCharacter* DeadCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BossDefeatWidgetClass;

	UFUNCTION(BlueprintCallable)
	void OnBossDefeated();
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float BossDefeatUIDelay = 2.0f;

protected:
	UPROPERTY()
	UUserWidget* BossDefeatWidget;

private:
	void ShowBossDefeatUI();

	FTimerHandle BossDefeatTimerHandle;
};
