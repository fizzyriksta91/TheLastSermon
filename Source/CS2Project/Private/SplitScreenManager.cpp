// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitScreenManager.h"

#include "SplitscreenGameViewportClient.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ASplitScreenManager::ASplitScreenManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASplitScreenManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASplitScreenManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Players.Num() < 2)
		return;

	APlayerController* PC0 = Players[0];
	APlayerController* PC1 = Players[1];
	if (!PC0 || !PC1)
		return;

	const FVector Loc0 = Pawn0->GetActorLocation();
	const FVector Loc1 = Pawn1->GetActorLocation();
	const float Distance = FVector::Dist(Loc0, Loc1);

	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (!ViewportClient)
		return;

	const bool bShouldSplit = Distance > SplitDistance;
	ViewportClient->SetForceDisableSplitscreen(!bShouldSplit);

	if (!bShouldSplit)
		return;

	USplitscreenGameViewportClient* SplitViewport = Cast<USplitscreenGameViewportClient>(ViewportClient);
	if (!SplitViewport || !PC0->PlayerCameraManager)
		return;

	// Use first player's camera orientation
	const FVector CameraForward = PC0->PlayerCameraManager->GetActorForwardVector();
	const FVector CameraRight   = PC0->PlayerCameraManager->GetActorRightVector();

	const FVector Delta = Loc1 - Loc0;

	const float ForwardDiff = FMath::Abs(FVector::DotProduct(Delta, CameraForward));
	const float RightDiff   = FMath::Abs(FVector::DotProduct(Delta, CameraRight));

	// More separation along forward axis -> horizontal (top/bottom). Else vertical (left/right).
	const ESplitScreenType::Type Desired =
		(ForwardDiff > RightDiff) ? ESplitScreenType::TwoPlayer_Horizontal
								  : ESplitScreenType::TwoPlayer_Vertical;

	if (SplitViewport->GetCurrentSplitscreenConfiguration() != Desired)
	{
		SplitViewport->SetActiveSplitscreenType(Desired);
	}
}


void ASplitScreenManager::SetPlayers(const TArray<APlayerController*>& InPlayers)
{
	Players = InPlayers;
}

