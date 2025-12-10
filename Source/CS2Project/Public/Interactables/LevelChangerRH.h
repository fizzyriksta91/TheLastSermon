// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableRH.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "LevelChangerRH.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UMediaPlayer;
class UMediaSoundComponent;
class UMediaSource;
class UMediaTexture;
class UUserWidget;

UCLASS()
class CS2PROJECT_API ALevelChangerRH : public AActor, public IInteractableRH
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChangerRH();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName LevelName;

	virtual void Interact_Implementation(ABaseCharacter* InteractingActor) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnCutsceneFinished();
	
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TSubclassOf<UUserWidget> CinematicWidgetClass;

	UPROPERTY()
	TArray<UUserWidget*> RuntimeWidgets;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* InteractionSphere;
	
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaSource* CutsceneSource;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaPlayer* EditorMediaPlayer;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	UMediaTexture* CutsceneTexture;

private:
	UPROPERTY()
	UMediaPlayer* RuntimeMediaPlayer;

	UPROPERTY()
	UMediaSoundComponent* RuntimeMediaSound;
	
	bool bUsingEditorMediaPlayer;
	
	UPROPERTY()
	TArray<APlayerController*> CachedPlayerControllers;
};
