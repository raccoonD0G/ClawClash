// Fill out your copyright notice in the Description page of Project Settings.


#include "CCStageMapManager.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/Managers/CCManagers.h"

#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/CCStageMap.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

#include "ClawClash/StageMap/StageMapParts/CCRoom.h"


UCCStageMapManager::UCCStageMapManager()
{
}

void UCCStageMapManager::Init()
{
    InitStageMapInfo();
    StageMap = NewObject<UCCStageMap>();
    StageMap->Init();
}

UCCStageMap& UCCStageMapManager::GetStageMap()
{
    return *StageMap;
}

void UCCStageMapManager::InitStageMapInfo()
{
    InitFieldRatioMap();
    InitFieldInfoMap();
    InitFeatureInfoMap();
}

void UCCStageMapManager::InitFieldRatioMap()
{
    // Initialize FieldRatioMap
    FieldRatioMap.Add(EFieldType::WatersideField, 0.1f);
    FieldRatioMap.Add(EFieldType::AsphaltField, 0.1f);
    FieldRatioMap.Add(EFieldType::CaveField, 0.1f);
    FieldRatioMap.Add(EFieldType::HillField, 0.00f);
    FieldRatioMap.Add(EFieldType::RaccoonHouseField, 0.1f);
    FieldRatioMap.Add(EFieldType::DogHouseField, 0.1f);
}

void UCCStageMapManager::InitFieldInfoMap()
{
    // Initialize FieldInfoMap

    FCCFieldInfo WatersideFieldInfo;
    WatersideFieldInfo.MaxLength = 15;
    WatersideFieldInfo.MinLength = 10;
    FieldInfoMap.Add(EFieldType::WatersideField, WatersideFieldInfo);

    FCCFieldInfo CaveFieldInfo;
    CaveFieldInfo.MaxLength = 15;
    CaveFieldInfo.MinLength = 10;
    FieldInfoMap.Add(EFieldType::CaveField, CaveFieldInfo);

    FCCFieldInfo AsphaltFieldInfo;
    AsphaltFieldInfo.MaxLength = 15;
    AsphaltFieldInfo.MinLength = 10;
    FieldInfoMap.Add(EFieldType::AsphaltField, AsphaltFieldInfo);

    FCCFieldInfo HillFieldInfo;
    HillFieldInfo.MaxLength = 12;
    HillFieldInfo.MinLength = 7;
    FieldInfoMap.Add(EFieldType::HillField, HillFieldInfo);

    FCCFieldInfo RaccoonHouseFieldInfo;
    RaccoonHouseFieldInfo.MaxLength = 15;
    RaccoonHouseFieldInfo.MinLength = 10;
    FieldInfoMap.Add(EFieldType::RaccoonHouseField, RaccoonHouseFieldInfo);

    FCCFieldInfo DogHouseFieldInfo;
    DogHouseFieldInfo.MaxLength = 15;
    DogHouseFieldInfo.MinLength = 10;
    FieldInfoMap.Add(EFieldType::DogHouseField, DogHouseFieldInfo);
}

void UCCStageMapManager::InitFeatureInfoMap()
{
    AddFeatureInfo<EWeedFeature>("/Game/Sprite/Map/Feature/Weed", FeatureInfoMap, EFeatureType::WeedFeature, 2.3f, 0.3f);
    AddFeatureInfo<ERockFeature>("/Game/Sprite/Map/Feature/Rock", FeatureInfoMap, EFeatureType::RockFeature, 8.0f, 0.3f);
    AddFeatureInfo<EBuildingFeature>("/Game/Sprite/Map/Feature/Building", FeatureInfoMap, EFeatureType::BuildingFeature, 1.3f, 0.3f);
    AddFeatureInfo<ECarFeature>("/Game/Sprite/Map/Feature/Car", FeatureInfoMap, EFeatureType::CarFeature, 2.3f, 0.3f);
    AddFeatureInfo<ELightFeature>("/Game/Sprite/Map/Feature/Light", FeatureInfoMap, EFeatureType::LightFeature, 2.3f, 0.3f);
    AddFeatureInfo<EStoneFeature>("/Game/Sprite/Map/Feature/Stone", FeatureInfoMap, EFeatureType::StoneFeature, 3.3f, 0.3f);
    AddFeatureInfo<EPlantFeature>("/Game/Sprite/Map/Feature/Plant", FeatureInfoMap, EFeatureType::PlantFeature, 1.3f, 0.3f);
    AddFeatureInfo<EGrassFeature>("/Game/Sprite/Map/Feature/Grass", FeatureInfoMap, EFeatureType::GrassFeature, 0.0f, 0.3f);
    AddFeatureInfo<EWaterFeature>("/Game/Sprite/Map/Feature/Water", FeatureInfoMap, EFeatureType::WaterFeature, 0.0f, 0.3f);
    AddFeatureInfo<ERatCaveFeature>("/Game/Sprite/Map/Feature/RatCave", FeatureInfoMap, EFeatureType::RatCaveFeature, 1.0f, 0.3f);
    AddFeatureInfo<ERaccooHouseFeature>("/Game/Sprite/Map/Feature/RaccoonHouse", FeatureInfoMap, EFeatureType::RacconHouseFeature, 1.0f, 0.001f);
    AddFeatureInfo<EDogHouseFeature>("/Game/Sprite/Map/Feature/DogHouse", FeatureInfoMap, EFeatureType::DogHouseFeature, 1.0f, 0.001f);
    AddFeatureInfo<ETreeFeature>("/Game/Sprite/Map/Feature/Tree", FeatureInfoMap, EFeatureType::TreeFeature, 0.1f, 1.0f);
}

template <typename TEnum>
void UCCStageMapManager::AddFeatureInfo(const FString& FolderPath, TMap<EFeatureType, FCCFeatureInfoArrContainer>& FeatureInfoMap, EFeatureType FeatureType, float NoneFeatureRatio, float FeatureRatio)
{
    TArray<FCCFeatureInfo> FeatureInfoArr;
    FeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<TEnum>()));
    FeatureInfoArr[(int32)TEnum::NoneFeature].FeatureRatio = NoneFeatureRatio;

    for (int32 i = 1; i < FeatureInfoArr.Num(); ++i)
    {
        FeatureInfoArr[i].FeatureRatio = FeatureRatio;
    }

    TArray<UPaperSprite*> SpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder(*FolderPath);

    for (int32 i = 0; i < SpriteArr.Num(); ++i)
    {
        FeatureInfoArr[i].FeatureSprite = SpriteArr[i];
    }

    FCCFeatureInfoArrContainer FeatureInfoArrContainer;
    FeatureInfoArrContainer.FeatureInfoArr = FeatureInfoArr;
    FeatureInfoMap.Add(FeatureType, FeatureInfoArrContainer);
}

