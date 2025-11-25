// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarRH.h"

#include "Components/ProgressBar.h"
#include "PlayerCharacters/BaseCharacter.h"

void UHealthBarRH::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UHealthBarRH::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateHealthBar();
}

void UHealthBarRH::UpdateHealthBar()
{
	if (!PlayerCharacter || !HealthBar) { return; }
	HealthBar->SetPercent(PlayerCharacter->GetHealthPercent());
}

void UHealthBarRH::SetPlayerCharacter(class ABaseCharacter* InCharacter)
{
	PlayerCharacter = InCharacter;
	
	if (!PlayerCharacter) return;

	// initialize percent
	const float Percent = PlayerCharacter->GetHealthPercent();
	if (HealthBar) HealthBar->SetPercent(Percent);

	// player HUD -> always visible; world (enemy) widget -> visible only when damaged
	const bool bIsPlayer = PlayerCharacter->GetController() && PlayerCharacter->GetController()->IsPlayerController();
	if (bIsPlayer)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(Percent < 1.0f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
