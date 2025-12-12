// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "CutscenePlayer.generated.h"

class UBoxComponent;
class UMediaPlayer;
class UMediaSoundComponent;
class UMediaSource;
class UMediaTexture;
class UUserWidget;

UCLASS()
class CS2PROJECT_API ACutscenePlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	ACutscenePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCutsceneFinished();

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaSource* CutsceneSource;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaPlayer* EditorMediaPlayer;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaTexture* CutsceneTexture;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TSubclassOf<UUserWidget> CinematicWidgetClass;
	
private:
	UPROPERTY()
	UMediaPlayer* RuntimeMediaPlayer;

	UPROPERTY()
	UMediaSoundComponent* RuntimeMediaSound;

	UPROPERTY()
	TArray<UUserWidget*> RuntimeWidgets;

	UPROPERTY()
	TArray<APlayerController*> CachedPlayerControllers;

	bool bUsingEditorMediaPlayer;
	
	bool bHasPlayed;
	
};
