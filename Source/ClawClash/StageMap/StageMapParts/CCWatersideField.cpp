// Fill out your copyright notice in the Description page of Project Settings.


#include "CCWatersideField.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"

void UCCWatersideField::CreateTile()
{
	Super::CreateTile();
	CreatNoneBasicFieldTile(EFieldType::WatersideField);
}

void UCCWatersideField::CreateSprite()
{
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::WeedFeature), true, true);
	PlaceSprites(0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::StoneFeature), true, true);
}
