// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawnManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/CCUtils.h"
#include "ClawClash/Spawner/CCSpawner.h"

UCCSpawnManager* UCCSpawnManager::Instance = nullptr;

UCCSpawnManager::UCCSpawnManager()
{
	
}

UCCSpawnManager* UCCSpawnManager::GetInstance()
{
	if (Instance == nullptr || !Instance->IsValidLowLevel())
	{
		Instance = NewObject<UCCSpawnManager>();
		Instance->Init();
	}
	return Instance;
}

void UCCSpawnManager::Init()
{
	RatSpriteArr = UCCUtils::GetAllResourceFromFolder<UPaperFlipbook>(TEXT("/Game/Sprite/NonPlayer/Rat"));
}

const TArray<UPaperFlipbook*>& UCCSpawnManager::GetRatSpriteArr() const
{
	return RatSpriteArr;
}

