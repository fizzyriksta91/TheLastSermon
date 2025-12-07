// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DodgeComponentRH.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CS2PROJECT_API UDodgeComponentRH : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeCooldown { 3.0f };
	
	float LastDodgeTime { -FLT_MAX };

	FTimerHandle DodgeTimerHandle;

public:	
	// Sets default values for this component's properties
	UDodgeComponentRH();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Dodge();

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* DodgeMontage;
	
	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bIsDodging { false };

	UPROPERTY(VisibleAnywhere, Category = "Dodge")
	bool bIsInvulnerable { false };

	UFUNCTION()
	void OnDodgeFinished();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* DodgeSound { nullptr };
};
