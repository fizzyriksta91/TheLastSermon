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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TEnumAsByte<EDamageTypesRH> DamageType;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetDamageType(TEnumAsByte<EDamageTypesRH> NewType);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* KickComboAnimMontage;
};
