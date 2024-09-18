// Fill out your copyright notice in the Description page of Project Settings.


#include "CCAsphaltField.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"

void UCCAsphaltField::CreateTile()
{
	Super::CreateTile();
	CreatNoneBasicFieldTile(EFieldType::AsphaltField);
}

void UCCAsphaltField::CreateSprite()
{
	Super::CreateSprite();
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::BuildingFeature), true, false);
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::CarFeature), true, true);
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::LightFeature), true, true);
}
