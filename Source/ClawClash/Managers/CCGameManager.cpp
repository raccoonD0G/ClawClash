// Fill out your copyright notice in the Description page of Project Settings.


#include "CCGameManager.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "CCGameManager.h"


UCCGameManager* UCCGameManager::Instance;


UCCGameManager* UCCGameManager::GetInstance()
{
    if (Instance == nullptr || !Instance->IsValidLowLevel())
    {
        Instance = NewObject<UCCGameManager>();
    }
    return Instance;
}



