// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ClawClash/StageMap/CCStageMapDef.h"

#include "ClawClash/CCUtils.h"

#include "PaperTileLayer.h"

#include "CCStageMapManager.generated.h"


USTRUCT()
struct FCCFieldInfo
{
    GENERATED_BODY()
public:
    UPROPERTY()
    int32 MinLength;

    UPROPERTY()
    int32 MaxLength;
};

USTRUCT()
struct FCCFeatureInfo
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TObjectPtr<class UPaperSprite> FeatureSprite;

    UPROPERTY()
    float FeatureRatio;

};

USTRUCT()
struct FCCFeatureInfoArrContainer
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TArray<FCCFeatureInfo> FeatureInfoArr;

    int32 GetRandomIndex() const
    {
        TArray<float> Probabilities;
        for (FCCFeatureInfo Info : FeatureInfoArr)
        {
            Probabilities.Add(Info.FeatureRatio);
        }
        return UCCUtils::GetRandomIndexByProbability(Probabilities);
    }
};

USTRUCT()
struct FCCFieldTileSet
{
    GENERATED_BODY()
    public:
    FPaperTileInfo LeftTile;
    FPaperTileInfo MiddleTile;
    FPaperTileInfo RightTile;
};

class UCCRoom;
class UCCPlatform;
class ACCTileMapActor;
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCStageMapManager : public UObject
{
	GENERATED_BODY()

public:
    UCCStageMapManager();
    static UCCStageMapManager* GetInstance();
    void Init();

protected:
    static UCCStageMapManager* Instance;




// Creat Section
protected:
    static ACCTileMapActor* StageMap;

public:
    static ACCTileMapActor* GetStageMap();
    static void SetStageMap(ACCTileMapActor* NewStageMap);

// Info Section
public:
    UPROPERTY()
    TMap<EFieldType, FCCFieldInfo> FieldInfoMap;

    UPROPERTY()
    TMap<EFieldType, float> FieldRatioMap;

    UPROPERTY()
    TMap<EFeatureType, FCCFeatureInfoArrContainer> FeatureInfoMap;

    UPROPERTY()
    TMap<EFieldType, FCCFieldTileSet> TileSetPerFieldDic;

    UPROPERTY()
    TMap<ETileType, FPaperTileInfo> TileInfoPerTileDic;

public:
    void InitializeTileSet(UPaperTileSet* NewFieldTileSet);

protected:
    void InitFieldRatioMap();
    void InitFieldInfoMap();
    void InitFeatureInfoMap();

    template <typename TEnum>
    void AddFeatureInfo(const FString& FolderPath, TMap<EFeatureType, FCCFeatureInfoArrContainer>& FeatureInfoMap, EFeatureType FeatureType, float NoneFeatureRatio, float FeatureRatio);

    void InitStageMapInfo();
};

template <typename TEnum>
void UCCStageMapManager::AddFeatureInfo(const FString& FolderPath, TMap<EFeatureType, FCCFeatureInfoArrContainer>& FeatureInfoMap, EFeatureType FeatureType, float NoneFeatureRatio, float FeatureRatio)
{
    TArray<FCCFeatureInfo> FeatureInfoArr;
    FeatureInfoArr.SetNum(UCCUtils::GetEnumLength(StaticEnum<TEnum>()));
    FeatureInfoArr[(int32)TEnum::NoneFeature].FeatureRatio = NoneFeatureRatio;

    for (int32 i = 1; i < FeatureInfoArr.Num(); ++i)
    {
        FeatureInfoArr[i].FeatureRatio = FeatureRatio;
    }

    TArray<UPaperSprite*> SpriteArr;
    SpriteArr.Add(nullptr);
    SpriteArr.Append(UCCUtils::GetAllResourceFromFolder<UPaperSprite>(*FolderPath));

    int32 Count = FMath::Min(SpriteArr.Num(), FeatureInfoArr.Num());
    for (int32 i = 1; i < Count; ++i)
    {
        FeatureInfoArr[i].FeatureSprite = SpriteArr[i];
    }

    FCCFeatureInfoArrContainer FeatureInfoArrContainer;
    FeatureInfoArrContainer.FeatureInfoArr = FeatureInfoArr;
    FeatureInfoMap.Add(FeatureType, FeatureInfoArrContainer);
}
