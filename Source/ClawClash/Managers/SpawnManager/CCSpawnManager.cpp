// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawnManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/CCUtils.h"

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
	FSpawnableFieldArrContainer DogFieldArr;
	SpawnFieldMap.Add(EFieldType::DogHouseField, DogFieldArr);

	FSpawnableFieldArrContainer RaccoonFieldArr;
	SpawnFieldMap.Add(EFieldType::RaccoonHouseField, RaccoonFieldArr);

	FSpawnableFieldArrContainer RatFieldArr;
	SpawnFieldMap.Add(EFieldType::CaveField, RatFieldArr);

	RatSpriteArr = UCCUtils::GetAllResourceFromFolder<UPaperFlipbook>(TEXT("/Game/Sprite/NonPlayer/Rat"));
}

