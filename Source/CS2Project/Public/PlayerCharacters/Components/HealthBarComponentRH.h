// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponentRH.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CS2PROJECT_API UHealthBarComponentRH : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponentRH();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HealthBar")
	TSubclassOf<UUserWidget> BossHealthWidgetClass;

	UPROPERTY()
	UUserWidget* BossHealthWidget;

	UPROPERTY(EditAnywhere, Category = "HealthBar")
	bool bUseViewportForBoss = true;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

private:
	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
	
};
