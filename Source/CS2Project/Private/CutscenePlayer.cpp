// Fill out your copyright notice in the Description page of Project Settings.


#include "CutscenePlayer.h"
#include "Components/BoxComponent.h"
#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

// Sets default values
ACutscenePlayer::ACutscenePlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(200.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACutscenePlayer::OnOverlapBegin);

	RuntimeMediaPlayer = nullptr;
	RuntimeMediaSound = nullptr;
	bUsingEditorMediaPlayer = false;
	RuntimeWidgets.Empty();
	CachedPlayerControllers.Empty();
	
	bHasPlayed = false;
}

// Called when the game starts or when spawned
void ACutscenePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACutscenePlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	// Only trigger for your player character type
	APawn* OverlapPawn = Cast<APawn>(OtherActor);
	if (!OverlapPawn)
		return;

	APlayerController* OverlapPC = Cast<APlayerController>(OverlapPawn->GetController());
	if (!OverlapPC || !OverlapPC->IsLocalController())
		return;
	
	//if (!OverlapPawn->IsA(ABaseCharacter::StaticClass()))
		//return;
	
	// If there's no cutscene source, do nothing (no level change)
	if (!CutsceneSource)
		return;
	
	if (bHasPlayed)
		return;

	if ((EditorMediaPlayer && EditorMediaPlayer->IsPlaying()) ||
		(RuntimeMediaPlayer && RuntimeMediaPlayer->IsPlaying()))
	{
		return;
	}
	// Cache local player controllers
	CachedPlayerControllers.Empty();
	if (GetWorld())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = Cast<APlayerController>(It->Get());
			if (PC && PC->GetLocalPlayer())
			{
				CachedPlayerControllers.Add(PC);
			}
		}
	}

	// Helper lambda to set up UI and disable input for each player controller
	auto SetupPlayerForCinematic = [&](APlayerController* PC)
	{
		if (!PC) return;

		// Fully disable movement/turning and hide player during cinematic
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
		PC->SetCinematicMode(true, true, true,
		                      true, true);
		PC->bShowMouseCursor = false;

		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->DisableInput(PC);
		}

		// Create widget owned by the player controller to avoid it being rendered into media textures/materials
		if (CinematicWidgetClass)
		{
			UUserWidget* W = CreateWidget<UUserWidget>(PC, CinematicWidgetClass);
			if (W)
			{
				W->AddToViewport();
				RuntimeWidgets.Add(W);
			}
		}
	};

	// If an editor MediaPlayer asset was assigned, prefer it
	if (EditorMediaPlayer)
	{
		if (!EditorMediaPlayer->CanPlaySource(CutsceneSource))
		{
			return;
		}

		EditorMediaPlayer->SetLooping(false);
		
		if (CutsceneTexture)
			CutsceneTexture->SetMediaPlayer(EditorMediaPlayer);

		RuntimeMediaSound = NewObject<UMediaSoundComponent>(this);
		if (RuntimeMediaSound)
		{
			RuntimeMediaSound->SetMediaPlayer(EditorMediaPlayer);
			RuntimeMediaSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			RuntimeMediaSound->RegisterComponent();
		}

		
		EditorMediaPlayer->OnEndReached.RemoveAll(this);
		EditorMediaPlayer->OnEndReached.AddUniqueDynamic(this, &ACutscenePlayer::OnCutsceneFinished);
		bUsingEditorMediaPlayer = true;

		if (EditorMediaPlayer->OpenSource(CutsceneSource))
		{
			const bool bStarted = EditorMediaPlayer->Play();
			if (bStarted)
			{
				bHasPlayed = true;

				RuntimeWidgets.Empty();
				for (APlayerController* PC : CachedPlayerControllers)
				{
					SetupPlayerForCinematic(PC);
				}
			}
			else
			{
				OnCutsceneFinished();
			}
		}
		else
		{
			OnCutsceneFinished();
		}
		return;
	}

	// Runtime media player fallback (no level change)
	RuntimeMediaPlayer = NewObject<UMediaPlayer>(this);
	if (!RuntimeMediaPlayer || !RuntimeMediaPlayer->CanPlaySource(CutsceneSource))
	{
		return;
	}
	
	RuntimeMediaPlayer->SetLooping(false);

	if (CutsceneTexture)
		CutsceneTexture->SetMediaPlayer(RuntimeMediaPlayer);

	RuntimeMediaSound = NewObject<UMediaSoundComponent>(this);
	if (RuntimeMediaSound)
	{
		RuntimeMediaSound->SetMediaPlayer(RuntimeMediaPlayer);
		RuntimeMediaSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		RuntimeMediaSound->RegisterComponent();
	}

	RuntimeMediaPlayer->OnEndReached.RemoveAll(this);
	RuntimeMediaPlayer->OnEndReached.AddUniqueDynamic(this, &ACutscenePlayer::OnCutsceneFinished);

	if (RuntimeMediaPlayer->OpenSource(CutsceneSource))
	{
		RuntimeMediaPlayer->Play();

		RuntimeWidgets.Empty();
		for (APlayerController* PC : CachedPlayerControllers)
		{
			SetupPlayerForCinematic(PC);
		}
	}
	else
	{
		OnCutsceneFinished();
	}
}

void ACutscenePlayer::OnCutsceneFinished()
{
	// Detach media texture
	if (CutsceneTexture)
	{
		CutsceneTexture->SetMediaPlayer(nullptr);
	}

	if (bUsingEditorMediaPlayer)
	{
		if (EditorMediaPlayer)
		{
			EditorMediaPlayer->OnEndReached.RemoveAll(this);
			
			if (EditorMediaPlayer->IsPlaying())
			{
				EditorMediaPlayer->Pause();
			}
			EditorMediaPlayer->Rewind();
		}
		bUsingEditorMediaPlayer = false;
	}
	else
	{
		if (RuntimeMediaPlayer)
		{
			RuntimeMediaPlayer->OnEndReached.RemoveAll(this);
			if (RuntimeMediaPlayer->IsPlaying())
			{
				RuntimeMediaPlayer->Close();
			}
			RuntimeMediaPlayer = nullptr;
		}
	}

	if (RuntimeMediaSound)
	{
		RuntimeMediaSound->SetMediaPlayer(nullptr);
		RuntimeMediaSound->UnregisterComponent();
		RuntimeMediaSound = nullptr;
	}

	for (UUserWidget* W : RuntimeWidgets)
	{
		if (W) W->RemoveFromParent();
	}
	RuntimeWidgets.Empty();

	// Re-enable input for all cached local player controllers
	for (APlayerController* PC : CachedPlayerControllers)
	{
		if (!PC) continue;

		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->SetCinematicMode(false, false, false, false, false);
		PC->bShowMouseCursor = false;

		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->EnableInput(PC);
		}
	}
	CachedPlayerControllers.Empty();
}

