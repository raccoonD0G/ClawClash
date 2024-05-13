// Fill out your copyright notice in the Description page of Project Settings.


#include "CCStageMapManager.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/Managers/CCManagers.h"

#include "ClawClash/TileMap/CCStageMap.h"
#include "ClawClash/TileMap/CCFloor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

UCCStageMapManager::UCCStageMapManager()
{
}

void UCCStageMapManager::Init()
{
    InitStageMapInfo();
}

void UCCStageMapManager::CreateStageMap()
{
    for (int32 i = 0; i < 3; i++)
    {
        TObjectPtr<UCCFloor> Floor;
        Floor = NewObject<UCCFloor>(this);
        Floor->Init(TileMapWidth, false);
        FloorArr.Add(Floor);
    }
    TObjectPtr<UCCFloor> ButtomFloor;
    ButtomFloor = NewObject<UCCFloor>(this);
    ButtomFloor->Init(TileMapWidth, true);
    FloorArr.Add(ButtomFloor);
}

void UCCStageMapManager::InitStageMapInfo()
{
    // Initialize FieldRatioMap
    FieldRatioMap.Add(EFieldType::WatersideField, 0.1f);
    FieldRatioMap.Add(EFieldType::AsphaltField, 0.1f);
    FieldRatioMap.Add(EFieldType::CaveField, 0.1f);
    FieldRatioMap.Add(EFieldType::HillField, 0.03f);

    // Initialize FieldInfoMap

    FCCFieldInfo WatersideFieldInfo;
    WatersideFieldInfo.MaxLength = 15;
    WatersideFieldInfo.MinLength = 10;

    FCCFieldInfo CaveFieldInfo;
    CaveFieldInfo.MaxLength = 15;
    CaveFieldInfo.MinLength = 10;

    FCCFieldInfo AsphaltFieldInfo;
    AsphaltFieldInfo.MaxLength = 15;
    AsphaltFieldInfo.MinLength = 10;

    FCCFieldInfo HillFieldInfo;
    HillFieldInfo.MaxLength = 12;
    HillFieldInfo.MinLength = 7;

    FieldInfoMap.Add(EFieldType::WatersideField, WatersideFieldInfo);
    FieldInfoMap.Add(EFieldType::CaveField, CaveFieldInfo);
    FieldInfoMap.Add(EFieldType::AsphaltField, AsphaltFieldInfo);
    FieldInfoMap.Add(EFieldType::HillField, HillFieldInfo);

    // Initialize FeatureInfoMap
    TArray<FCCFeatureInfo> WeedFeatureInfoArr;
    WeedFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EWeedFeature>()));
    WeedFeatureInfoArr[(int32)EWeedFeature::NoneFeature].FeatureRatio = 2.3f;
    WeedFeatureInfoArr[(int32)EWeedFeature::Weed0Feature].FeatureRatio = 0.3f;
    WeedFeatureInfoArr[(int32)EWeedFeature::Weed1Feature].FeatureRatio = 0.3f;
    WeedFeatureInfoArr[(int32)EWeedFeature::Weed2Feature].FeatureRatio = 0.3f;
    WeedFeatureInfoArr[(int32)EWeedFeature::Weed3Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> WeedSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Weed");
    if (WeedFeatureInfoArr.Num() != WeedSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < WeedSpriteArr.Num(); i++)
    {
        WeedFeatureInfoArr[i].FeatureSprite = WeedSpriteArr[i];
    }

    TArray<FCCFeatureInfo> RockFeatureInfoArr;
    RockFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<ERockFeature>()));
    RockFeatureInfoArr[(int32)ERockFeature::NoneFeature].FeatureRatio = 2.3f;
    RockFeatureInfoArr[(int32)ERockFeature::Rock0Feature].FeatureRatio = 0.3f;
    RockFeatureInfoArr[(int32)ERockFeature::Rock1Feature].FeatureRatio = 0.3f;
    RockFeatureInfoArr[(int32)ERockFeature::Rock2Feature].FeatureRatio = 0.3f;
    RockFeatureInfoArr[(int32)ERockFeature::Rock3Feature].FeatureRatio = 0.3f;
    RockFeatureInfoArr[(int32)ERockFeature::Rock4Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> RockSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Rock");
    if (RockFeatureInfoArr.Num() != RockSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < RockSpriteArr.Num(); i++)
    {
        RockFeatureInfoArr[i].FeatureSprite = RockSpriteArr[i];
    }

    TArray<FCCFeatureInfo> BuildingFeatureInfoArr;
    BuildingFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EBuildingFeature>()));
    BuildingFeatureInfoArr[(int32)EBuildingFeature::NoneFeature].FeatureRatio = 1.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House0Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House1Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House2Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House3Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House4Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House5Feature].FeatureRatio = 0.3f;
    BuildingFeatureInfoArr[(int32)EBuildingFeature::House6Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> BuildingSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Building");
    if (BuildingFeatureInfoArr.Num() != BuildingSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < BuildingSpriteArr.Num(); i++)
    {
        BuildingFeatureInfoArr[i].FeatureSprite = BuildingSpriteArr[i];
    }

    TArray<FCCFeatureInfo> CarFeatureInfoArr;
    CarFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<ECarFeature>()));
    CarFeatureInfoArr[(int32)ECarFeature::NoneFeature].FeatureRatio = 2.3f;
    CarFeatureInfoArr[(int32)ECarFeature::Car0Feature].FeatureRatio = 0.3f;
    CarFeatureInfoArr[(int32)ECarFeature::Car1Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> CarSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Car");
    if (CarFeatureInfoArr.Num() != CarSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < CarSpriteArr.Num(); i++)
    {
        CarFeatureInfoArr[i].FeatureSprite = CarSpriteArr[i];
    }

    TArray<FCCFeatureInfo> LightFeatureInfoArr;
    LightFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<ELightFeature>()));
    LightFeatureInfoArr[(int32)ELightFeature::NoneFeature].FeatureRatio = 2.3f;
    LightFeatureInfoArr[(int32)ELightFeature::ShortLightFeature].FeatureRatio = 0.3f;
    LightFeatureInfoArr[(int32)ELightFeature::MiddleLightFeature].FeatureRatio = 0.3f;
    LightFeatureInfoArr[(int32)ELightFeature::LongLightFeature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> LightSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Light");
    if (LightFeatureInfoArr.Num() != LightSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < LightSpriteArr.Num(); i++)
    {
        LightFeatureInfoArr[i].FeatureSprite = LightSpriteArr[i];
    }

    TArray<FCCFeatureInfo> StoneFeatureInfoArr;
    StoneFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EStoneFeature>()));
    StoneFeatureInfoArr[(int32)EStoneFeature::NoneFeature].FeatureRatio = 3.3f;
    StoneFeatureInfoArr[(int32)EStoneFeature::Stone0Feature].FeatureRatio = 0.3f;
    StoneFeatureInfoArr[(int32)EStoneFeature::Stone1Feature].FeatureRatio = 0.3f;
    StoneFeatureInfoArr[(int32)EStoneFeature::Stone2Feature].FeatureRatio = 0.3f;
    StoneFeatureInfoArr[(int32)EStoneFeature::Stone3Feature].FeatureRatio = 0.3f;
    StoneFeatureInfoArr[(int32)EStoneFeature::Stone4Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> StonSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Stone");
    if (StoneFeatureInfoArr.Num() != StonSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < StonSpriteArr.Num(); i++)
    {
        StoneFeatureInfoArr[i].FeatureSprite = StonSpriteArr[i];
    }


    TArray<FCCFeatureInfo> PlantFeatureInfoArr;
    PlantFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EPlantFeature>()));
    PlantFeatureInfoArr[(int32)EPlantFeature::NoneFeature].FeatureRatio = 1.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant0Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant1Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant2Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant3Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant4Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant5Feature].FeatureRatio = 0.3f;
    PlantFeatureInfoArr[(int32)EPlantFeature::Plant6Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> PlantSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Plant");
    if (PlantFeatureInfoArr.Num() != PlantSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < PlantSpriteArr.Num(); i++)
    {
        PlantFeatureInfoArr[i].FeatureSprite = PlantSpriteArr[i];
    }

    TArray<FCCFeatureInfo> GrassFeatureInfoArr;
    GrassFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EGrassFeature>()));
    GrassFeatureInfoArr[(int32)EGrassFeature::NoneFeature].FeatureRatio = 0.0f;
    GrassFeatureInfoArr[(int32)EGrassFeature::Grass0Feature].FeatureRatio = 0.3f;
    GrassFeatureInfoArr[(int32)EGrassFeature::Grass1Feature].FeatureRatio = 0.3f;
    GrassFeatureInfoArr[(int32)EGrassFeature::Grass2Feature].FeatureRatio = 0.3f;
    GrassFeatureInfoArr[(int32)EGrassFeature::Grass3Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> GrassSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Grass");
    if (GrassFeatureInfoArr.Num() != GrassSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < GrassSpriteArr.Num(); i++)
    {
        GrassFeatureInfoArr[i].FeatureSprite = GrassSpriteArr[i];
    }

    TArray<FCCFeatureInfo> WaterFeatureInfoArr;
    WaterFeatureInfoArr.SetNum(UCCManagers::GetInstance()->GetEnumLength(StaticEnum<EWaterFeature>()));
    WaterFeatureInfoArr[(int32)EWaterFeature::NoneFeature].FeatureRatio = 0.0f;
    WaterFeatureInfoArr[(int32)EWaterFeature::Water0Feature].FeatureRatio = 0.3f;
    TArray<UPaperSprite*> WaterSpriteArr = UCCManagers::GetInstance()->GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Water");
    if (WaterFeatureInfoArr.Num() != WaterSpriteArr.Num())
    {
        UE_LOG(LogTemp, Log, TEXT("FeatureRatio Length and FeatureSprite Length Not Same"));
    }
    for (int32 i = 0; i < WaterSpriteArr.Num(); i++)
    {
        WaterFeatureInfoArr[i].FeatureSprite = WaterSpriteArr[i];
    }

    FCCFeatureInfoArrContainer BuildingFeatureInfoArrContainer;
    BuildingFeatureInfoArrContainer.FeatureInfoArr = BuildingFeatureInfoArr;
    FCCFeatureInfoArrContainer CarFeatureInfoArrContainer;
    CarFeatureInfoArrContainer.FeatureInfoArr = CarFeatureInfoArr;
    FCCFeatureInfoArrContainer WeedFeatureInfoArrContainer;
    WeedFeatureInfoArrContainer.FeatureInfoArr = WeedFeatureInfoArr;
    FCCFeatureInfoArrContainer LightFeatureInfoArrContainer;
    LightFeatureInfoArrContainer.FeatureInfoArr = LightFeatureInfoArr;
    FCCFeatureInfoArrContainer RockFeatureInfoArrContainer;
    RockFeatureInfoArrContainer.FeatureInfoArr = RockFeatureInfoArr;
    FCCFeatureInfoArrContainer StoneFeatureInfoArrContainer;
    StoneFeatureInfoArrContainer.FeatureInfoArr = StoneFeatureInfoArr;
    FCCFeatureInfoArrContainer PlantFeatureInfoArrContainer;
    PlantFeatureInfoArrContainer.FeatureInfoArr = PlantFeatureInfoArr;
    FCCFeatureInfoArrContainer GrassFeatureInfoArrContainer;
    GrassFeatureInfoArrContainer.FeatureInfoArr = GrassFeatureInfoArr;
    FCCFeatureInfoArrContainer WaterFeatureInfoArrContainer;
    WaterFeatureInfoArrContainer.FeatureInfoArr = WaterFeatureInfoArr;

    FeatureInfoMap.Add(EFeatureType::BuildingFeature, BuildingFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::CarFeature, CarFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::WeedFeature, WeedFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::LightFeature, LightFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::RockFeature, RockFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::StoneFeature, StoneFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::PlantFeature, PlantFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::GrassFeature, GrassFeatureInfoArrContainer);
    FeatureInfoMap.Add(EFeatureType::WaterFeature, WaterFeatureInfoArrContainer);

}
