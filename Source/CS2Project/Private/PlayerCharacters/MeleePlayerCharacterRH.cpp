// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/MeleePlayerCharacterRH.h"

#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

AMeleePlayerCharacterRH::AMeleePlayerCharacterRH()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeCombatComp = CreateDefaultSubobject<UMeleeCombatComponentRH>(TEXT("Melee Combat Component"));
	LockonComp = CreateDefaultSubobject<ULockOnComponentRH>(TEXT("Lockon Component"));
}
