// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LanternDamageComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API ULanternDamageComponentRH : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULanternDamageComponentRH();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "Lantern")
	bool bIsInLight { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
	float DarknessTickDamage { 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
	float DamageTickrate { 1.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Lantern")
	TArray<AActor*> ActiveLanterns;

	UFUNCTION(BlueprintCallable, Category = "Lantern")
	void EnterLight(AActor* Lantern);

	UFUNCTION(BlueprintCallable, Category = "Lantern")
	void ExitLight(AActor* Lantern);

private:
	UPROPERTY()
	ACharacter* CharacterRef;

	FTimerHandle DamageTimerHandle;

	void ApplyDarknessDamage();
	
	void UpdateLightStatus();
};
