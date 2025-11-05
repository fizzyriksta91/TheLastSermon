// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBaseCharacter.h"
#include "BossCharacterRH.generated.h"

/**
 * 
 */
UCLASS()
class CS2PROJECT_API ABossCharacterRH : public AEnemyBaseCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformKickCombo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* KickComboAnimMontage;
};
