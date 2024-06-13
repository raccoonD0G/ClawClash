// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawnManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "ClawClash/Managers/CCManagers.h"

UCCSpawnManager::UCCSpawnManager()
{
	
}

void UCCSpawnManager::Init()
{
	FSpawnableFieldArrContainer DogFieldArr;
	SpawnFieldMap.Add(EFieldType::DogHouseField, DogFieldArr);

	FSpawnableFieldArrContainer RaccoonFieldArr;
	SpawnFieldMap.Add(EFieldType::RaccoonHouseField, RaccoonFieldArr);

	FSpawnableFieldArrContainer RatFieldArr;
	SpawnFieldMap.Add(EFieldType::CaveField, RatFieldArr);

	RatSpriteArr = UCCManagers::GetInstance()->GetAllResourceFromFolder<UPaperFlipbook>(TEXT("/Game/Sprite/NonPlayer/Rat"));
}

