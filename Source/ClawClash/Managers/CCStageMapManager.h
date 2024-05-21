// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ClawClash/TileMap/CCStageMap.h"

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
};


/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCStageMapManager : public UObject
{
	GENERATED_BODY()

public:
    UCCStageMapManager();
    void Init();

protected:

 // Layer Section
public:
    const float BackgroundY = -200;
    const float PlayerY = 50;
    const float FieldTileY = 50.1f;

// Creat Section
public:
    UPROPERTY()
    TArray<TObjectPtr<class UCCFloor>> FloorArr;

    void CreateStageMap();

// Info Section
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapWidth = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapHeight = 64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 NumOfFloor = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileWidth = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileHeight = 512;

    UPROPERTY()
    TMap<EFieldType, FCCFieldInfo> FieldInfoMap;

    UPROPERTY()
    TMap<EFieldType, float> FieldRatioMap;

    UPROPERTY()
    TMap<EFeatureType, FCCFeatureInfoArrContainer> FeatureInfoMap;

protected:
    void InitFieldRatioMap();
    void InitFieldInfoMap();
    void InitFeatureInfoMap();
    template <typename TEnum>
    void AddFeatureInfo(const FString& FolderPath, TMap<EFeatureType, FCCFeatureInfoArrContainer>& FeatureInfoMap, EFeatureType FeatureType, float NoneFeatureRatio, float FeatureRatio);
    void InitStageMapInfo();
};
