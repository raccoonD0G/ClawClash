// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRat.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "PaperFlipbookComponent.h"

void ACCPaperRat::BeginPlay()
{
	Super::BeginPlay();
	int32 Index = FMath::RandRange(0, UCCManagers::GetInstance()->GetSpawnManager()->RatSpriteArr.Num() - 1);
	PlayerFlipbook->SetFlipbook(UCCManagers::GetInstance()->GetSpawnManager()->RatSpriteArr[Index]);
}
