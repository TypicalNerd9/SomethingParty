// Fill out your copyright notice in the Description page of Project Settings.


#include "SomethingPartyPlayerState.h"

void ASomethingPartyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	WaitingToRoll = true;
}
