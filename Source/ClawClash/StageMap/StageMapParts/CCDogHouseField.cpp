// Fill out your copyright notice in the Description page of Project Settings.


#include "CCDogHouseField.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"

void UCCDogHouseField::CreateTile()
{
    Super::CreateTile();
    CreatNoneBasicFieldTile(EFieldType::WatersideField);
}

void UCCDogHouseField::CreateSprite()
{
    Super::CreateSprite();
    PlaceSprites(1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::DogHouseFeature), true, true, false, 1, 1);
    PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}

