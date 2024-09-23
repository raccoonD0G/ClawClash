// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRat.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "PaperFlipbookComponent.h"

ACCPaperRat::ACCPaperRat() : Super()
{
	IsRightStart = true;
}

void ACCPaperRat::BeginPlay()
{
	Super::BeginPlay();
	int32 Index = FMath::RandRange(0, UCCSpawnManager::GetInstance()->GetRatSpriteArr().Num() - 1);
	GetSprite()->SetFlipbook(UCCSpawnManager::GetInstance()->GetRatSpriteArr()[Index]);
}

void ACCPaperRat::StartMove()
{
	Super::StartMove();
}

void ACCPaperRat::EndMove()
{
	Super::EndMove();
}
