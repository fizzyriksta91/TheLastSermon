// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "RangePlayerCharacterRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API ARangePlayerCharacterRH : public ABaseCharacter
{
	GENERATED_BODY()

public:

	ARangePlayerCharacterRH();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class URangeCombatComponentRH* RangeCombatComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class ULanternDamageComponentRH* LanternDamageComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	class ULockOnComponentRH* LockOnComp;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lantern")
	TSubclassOf<AActor> LanternBlueprintClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lantern")
	FName LanternAttachSocket = NAME_None;

private:
	UPROPERTY()
	AActor* SpawnedLantern = nullptr;
	
};
