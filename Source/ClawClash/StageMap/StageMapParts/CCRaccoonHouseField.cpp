// Fill out your copyright notice in the Description page of Project Settings.


#include "CCRaccoonHouseField.h"

void UCCRaccoonHouseField::CreateTile()
{
	Super::CreateTile();
	CreatNoneBasicFieldTile(EFieldType::RaccoonHouseField);
}

void UCCRaccoonHouseField::CreateSprite()
{
	Super::CreateSprite();
	PlaceSprites(1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RacconHouseFeature), true, true, false, 1, 1);
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}
