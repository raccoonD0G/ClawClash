// Fill out your copyright notice in the Description page of Project Settings.


#include "CCCaveField.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"

void UCCCaveField::CreateTile()
{
    Super::CreateTile();
    CreatNoneBasicFieldTile(EFieldType::CaveField);
    FSpawnableField NewField;
    NewField.SpawnableType = ESpawnableType::Rat;
    NewField.LeftEnd = FVector(TileMapPos.X * UCCStageMapManager::GetStageMap()->GetTileWidth(), 0, -TileMapPos.Y * UCCStageMapManager::GetStageMap()->GetTileHeight() + UCCStageMapManager::GetStageMap()->GetTileHeight() / 2);
    NewField.RightEnd = FVector((TileMapPos.X + Length - 1) * UCCStageMapManager::GetStageMap()->GetTileWidth(), 0, -TileMapPos.Y * UCCStageMapManager::GetStageMap()->GetTileHeight() + UCCStageMapManager::GetStageMap()->GetTileHeight() / 2);
    NewField.MaxCharacterNum = Length;
    UCCSpawnManager::GetInstance()->SpawnFieldMap.Find(EFieldType::CaveField)->SpawnableFieldArr.Add(NewField);
}

void UCCCaveField::CreateSprite()
{
    Super::CreateSprite();
    PlaceSprites(1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RatCaveFeature), true, true, false, 4, 4);
    PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}