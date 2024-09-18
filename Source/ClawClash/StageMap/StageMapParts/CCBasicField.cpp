// Fill out your copyright notice in the Description page of Project Settings.


#include "CCBasicField.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"

void UCCBasicField::CreateTile()
{
    Super::CreateTile();

    for (int32 i = TileMapPos.X; i < TileMapPos.X + Length; i++)
    {
        OwningTileMap->SetTileIfPossible(i, TileMapPos.Y, 0, *UCCStageMapManager::GetInstance()->TileInfoPerTileDic.Find(ETileType::Basic));
    }
    OwningTileMap->SetupTileColliders(TileMapPos.X, TileMapPos.Y, Length, FieldType);
}

void UCCBasicField::CreateSprite()
{
    Super::CreateSprite();
    PlaceSprites(0.5f, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::GrassFeature), false, true);
    PlaceSprites(0.5f, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::PlantFeature), false, true);
}
