// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatRH.h"
#include "PlayerCharacters/Enums/EDamageTypesRH.h"
#include "BaseCharacter.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, ABaseCharacter*, DeadCharacter);

UCLASS()
class CS2PROJECT_API ABaseCharacter : public ACharacter , public ICombatRH
{
	GENERATED_BODY()

	

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStatsComponentRH* StatsComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponentRH* TraceComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UDodgeComponentRH* DodgeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat")
	TMap<TEnumAsByte<EDamageTypesRH>, float> DamageValues;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Makes the character face its movement direction
	void FaceMovementDirection();

	FRotator LastMovementRotation;

	bool bHasMovedBefore { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay { 5.f };

	FTimerHandle DeathTimerHandle;

	UFUNCTION()
	void DestroyCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage(EDamageTypesRH DamageType) override;

	virtual float TakeDamage(
		float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnCharacterDeath OnCharacterDeath;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void OnDeath();

	virtual bool IsDead() const override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;
	
};
