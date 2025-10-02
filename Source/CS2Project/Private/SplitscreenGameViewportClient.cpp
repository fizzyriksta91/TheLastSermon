// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitscreenGameViewportClient.h"

void USplitscreenGameViewportClient::SetActiveSplitscreenType(ESplitScreenType::Type DynamicSplitscreen)
{
	ActiveSplitscreenType = DynamicSplitscreen;
	bDisableSplitscreenOverride = (DynamicSplitscreen == ESplitScreenType::None);
	LayoutPlayers();
}

