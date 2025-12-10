// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/LevelChangerRH.h"

#include "FileMediaSource.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ALevelChangerRH::ALevelChangerRH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = SceneRoot;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMesh->SetupAttachment(RootComponent);
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(StaticMesh);
	InteractionSphere->SetSphereRadius(200.f);
	
	RuntimeMediaPlayer = nullptr;
	RuntimeMediaSound = nullptr;
	bUsingEditorMediaPlayer = false;
	RuntimeWidgets.Empty();
    CachedPlayerControllers.Empty();
}

// Interaction for changing levels
void ALevelChangerRH::Interact_Implementation(ABaseCharacter* InteractingActor)
{
	IInteractableRH::Interact_Implementation(InteractingActor);

	if (!InteractingActor)
		return;

	if (LevelName.IsNone())
		return;

	if (!CutsceneSource)
	{
		UGameplayStatics::OpenLevel(this, LevelName);
		return;
	}

	CachedPlayerControllers.Empty();
	if (GetWorld())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = Cast<APlayerController>(It->Get());
			if (PC && PC->GetLocalPlayer()) // only local players
			{
				CachedPlayerControllers.Add(PC);
			}
		}
	}
	
	// If an editor MediaPlayer asset was assigned, use it so your existing MediaTexture/Material shows video
	if (EditorMediaPlayer)
	{
		// Ensure the media player can play the source
		if (!EditorMediaPlayer->CanPlaySource(CutsceneSource))
		{
			UGameplayStatics::OpenLevel(this, LevelName);
			return;
		}

		// Attach media texture (if set) to the editor player so the material updates
		if (CutsceneTexture)
		{
			CutsceneTexture->SetMediaPlayer(EditorMediaPlayer);
		}

		// Create a sound component to play audio through
		RuntimeMediaSound = NewObject<UMediaSoundComponent>(this);
		if (RuntimeMediaSound)
		{
			RuntimeMediaSound->SetMediaPlayer(EditorMediaPlayer);
			RuntimeMediaSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			RuntimeMediaSound->RegisterComponent();
		}

		// Bind to end event and play
		EditorMediaPlayer->OnEndReached.AddUniqueDynamic(this, &ALevelChangerRH::OnCutsceneFinished);
		bUsingEditorMediaPlayer = true;

		if (EditorMediaPlayer->OpenSource(CutsceneSource))
		{
			EditorMediaPlayer->Play();

			// Create per-player widgets and disable input for each cached controller
			RuntimeWidgets.Empty();
			for (APlayerController* PC : CachedPlayerControllers)
			{
				if (!PC)
					continue;

				// disable controller-level input (stop movement, look, HUD, etc.)
				PC->SetIgnoreMoveInput(true);
				PC->SetIgnoreLookInput(true);
				PC->SetCinematicMode(true /*bInCinematicMode*/, false /*bDisableMovement*/, false /*bDisableTurning*/,
									  true /*bHidePlayer*/, true /*bDisableCameraCuts*/);

				// also disable the pawn's input so character can't move
				if (APawn* Pawn = PC->GetPawn())
				{
					Pawn->DisableInput(PC);
				}

				// Create an unowned widget and add to the viewport (not the player's render target)
				if (CinematicWidgetClass)
				{
					UUserWidget* W = CreateWidget<UUserWidget>(GetWorld(), CinematicWidgetClass);
					if (W)
					{
						W->AddToViewport(); 
						RuntimeWidgets.Add(W);
					}
				}
			}
		}
		else
		{
			OnCutsceneFinished();
		}
		return;
	}

	// Fallback: create a runtime media player and attach the existing MediaTexture to it
	RuntimeMediaPlayer = NewObject<UMediaPlayer>(this);
	if (!RuntimeMediaPlayer || !RuntimeMediaPlayer->CanPlaySource(CutsceneSource))
	{
		UGameplayStatics::OpenLevel(this, LevelName);
		return;
	}

	// Attach runtime media texture if you assigned one in the actor instance
	if (CutsceneTexture)
	{
		CutsceneTexture->SetMediaPlayer(RuntimeMediaPlayer);
	}

	RuntimeMediaSound = NewObject<UMediaSoundComponent>(this);
	if (RuntimeMediaSound)
	{
		RuntimeMediaSound->SetMediaPlayer(RuntimeMediaPlayer);
		RuntimeMediaSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		RuntimeMediaSound->RegisterComponent();
	}

	RuntimeMediaPlayer->OnEndReached.AddUniqueDynamic(this, &ALevelChangerRH::OnCutsceneFinished);

	if (RuntimeMediaPlayer->OpenSource(CutsceneSource))
	{
		RuntimeMediaPlayer->Play();

		// Create per-player widgets and disable input for each cached controller
		RuntimeWidgets.Empty();
		for (APlayerController* PC : CachedPlayerControllers)
		{
			if (!PC)
				continue;

			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
			PC->SetCinematicMode(true /*bInCinematicMode*/, false /*bDisableMovement*/, false /*bDisableTurning*/,
								 true /*bHidePlayer*/, true /*bDisableCameraCuts*/);

			if (APawn* Pawn = PC->GetPawn())
			{
				Pawn->DisableInput(PC);
			}

			if (CinematicWidgetClass)
			{
				UUserWidget* W = CreateWidget<UUserWidget>(GetWorld(), CinematicWidgetClass);
				if (W)
				{
					W->AddToViewport();
					RuntimeWidgets.Add(W);
				}
			}
		}
	}
	else
	{
		OnCutsceneFinished();
	}
}

// Called when the game starts or when spawned
void ALevelChangerRH::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelChangerRH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelChangerRH::OnCutsceneFinished()
{
	// Detach media texture from whichever player we used
	if (CutsceneTexture)
	{
		CutsceneTexture->SetMediaPlayer(nullptr);
	}

	if (bUsingEditorMediaPlayer)
	{
		if (EditorMediaPlayer)
		{
			EditorMediaPlayer->OnEndReached.RemoveAll(this);
			// Do not Close the editor asset to avoid unintended side-effects in editor; we only unbind.
		}
		bUsingEditorMediaPlayer = false;
	}
	else
	{
		if (RuntimeMediaPlayer)
		{
			RuntimeMediaPlayer->OnEndReached.RemoveAll(this);
			RuntimeMediaPlayer->Close();
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
		if (W)
		{
			W->RemoveFromParent();
		}
	}
	RuntimeWidgets.Empty();

	// Re-enable input for all cached local player controllers
	for (APlayerController* PC : CachedPlayerControllers)
	{
		if (!PC)
			continue;

		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->SetCinematicMode(false /*bInCinematicMode*/, false, false, false, false);

		if (APawn* Pawn = PC->GetPawn())
		{
			Pawn->EnableInput(PC);
		}
	}
	CachedPlayerControllers.Empty();

	// Open the target level
	if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

