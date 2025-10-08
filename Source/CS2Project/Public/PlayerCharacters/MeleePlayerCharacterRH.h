// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacters/BaseCharacter.h"
#include "MeleePlayerCharacterRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API AMeleePlayerCharacterRH : public ABaseCharacter
{
	GENERATED_BODY()
public:

	AMeleePlayerCharacterRH();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMeleeCombatComponentRH* MeleeCombatComp;
};
