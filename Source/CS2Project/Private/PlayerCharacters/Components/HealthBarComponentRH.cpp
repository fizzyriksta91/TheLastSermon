// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/Components/HealthBarComponentRH.h"

#include "Components/ProgressBar.h"
#include "PlayerCharacters/BaseCharacter.h"

UHealthBarComponentRH::UHealthBarComponentRH()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(150.0f, 10.0f));
	SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

void UHealthBarComponentRH::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter) { return; }

	// If owner is a boss and a boss widget class is provided, create HUD widget
	if (bUseViewportForBoss && OwnerCharacter->IsBoss() && BossHealthWidgetClass)
	{
		BossHealthWidget = CreateWidget<UUserWidget>(GetWorld(), BossHealthWidgetClass);
		if (BossHealthWidget)
		{
			BossHealthWidget->AddToViewport();
			// hide the world widget component so the floating bar doesn't show
			SetHiddenInGame(true);
		}
	}
	UpdateHealthBar();
}

void UHealthBarComponentRH::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHealthBar();
}

void UHealthBarComponentRH::UpdateHealthBar()
{
	if (!OwnerCharacter) { return; }

	if (bUseViewportForBoss && OwnerCharacter->IsBoss())
	{
		if (!BossHealthWidget && BossHealthWidgetClass)
		{
			BossHealthWidget = CreateWidget<UUserWidget>(GetWorld(), BossHealthWidgetClass);
			if (BossHealthWidget) BossHealthWidget->AddToViewport();
		}

		if (!BossHealthWidget) { return; }

		UProgressBar* HealthBar = Cast<UProgressBar>(BossHealthWidget->GetWidgetFromName(TEXT("HealthBar")));
		if (HealthBar)
		{
			HealthBar->SetPercent(OwnerCharacter->GetHealthPercent());
		}
		return;
	}
    
	UUserWidget* HealthWidget = GetWidget();
	if (!HealthWidget) { return; }
	
	UProgressBar* HealthBar = Cast<UProgressBar>(
		HealthWidget->GetWidgetFromName(TEXT("HealthBar")));
    
	if (HealthBar)
	{
		HealthBar->SetPercent(OwnerCharacter->GetHealthPercent());
	}
}


