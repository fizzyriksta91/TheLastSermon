// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacters/MeleePlayerCharacterRH.h"

#include "PlayerCharacters/Components/LanternDamageComponentRH.h"
#include "PlayerCharacters/Components/LockOnComponentRH.h"
#include "PlayerCharacters/Components/MeleeCombatComponentRH.h"

AMeleePlayerCharacterRH::AMeleePlayerCharacterRH()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeCombatComp = CreateDefaultSubobject<UMeleeCombatComponentRH>(TEXT("Melee Combat Component"));
	LanternDamageComp = CreateDefaultSubobject<ULanternDamageComponentRH>(TEXT("Lantern Damage Component"));
	LockOnComp = CreateDefaultSubobject<ULockOnComponentRH>(TEXT("Lock On Component"));
}
