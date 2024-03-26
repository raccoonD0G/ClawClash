// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlayerController.h"
#include "CCPaperPlayer.h"
#include "EngineUtils.h"

void ACCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}
