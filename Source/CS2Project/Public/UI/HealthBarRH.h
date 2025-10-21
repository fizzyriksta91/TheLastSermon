// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API UHealthBarRH : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

	UFUNCTION(BlueprintCallable)
	void SetPlayerCharacter(class ABaseCharacter* InCharacter);

private:
	UPROPERTY()
	class ABaseCharacter* PlayerCharacter;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
