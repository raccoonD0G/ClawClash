// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRat.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "PaperFlipbookComponent.h"

void ACCPaperRat::BeginPlay()
{
	Super::BeginPlay();
	int32 Index = FMath::RandRange(0, UCCSpawnManager::GetInstance()->RatSpriteArr.Num() - 1);
	PlayerFlipbook->SetFlipbook(UCCSpawnManager::GetInstance()->RatSpriteArr[Index]);
}
