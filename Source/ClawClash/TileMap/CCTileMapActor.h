// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "ClawClash/TileMap/CCStageMap.h"
#include "CCTileMapActor.generated.h"

class UPaperTileMap;

USTRUCT()
struct FCCFieldTileSet
{
    GENERATED_BODY()
 public:
     FPaperTileInfo LeftTile;
     FPaperTileInfo MiddleTile;
     FPaperTileInfo RightTile;
};

UCLASS()
class CLAWCLASH_API ACCTileMapActor : public AActor
{
    GENERATED_BODY()

public:
    ACCTileMapActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

protected:
    const float BackgroundY = -200;
    const float PlayerY = 50;
    const float FieldTileY = 200;
    float CurrentBeforePlayerFeatureY;
    float CurrentAfterPlayerFeatureY;

// Background Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UPaperSpriteComponent> BackgroundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TObjectPtr<class UPaperSprite> BackGroundSprite;

    UPROPERTY()
    TObjectPtr<class ACharacter> Player;

    int32 LastPlayerX;
    int32 LastPlayerZ;

    int32 LastBackgroundX;
    int32 LastBackgroundZ;

// Sprite Section
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TObjectPtr<class UMaterialInterface> DefaultSpriteMaterial;

// TileMap Section
protected:
    // Create Field
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> FieldTileSet;

    UPROPERTY()
    TMap<EFieldType, FCCFieldTileSet> TileSetPerFieldDic;

    UPROPERTY()
    TMap<ETileType, FPaperTileInfo> TileInfoPerTileDic;

    UFUNCTION()
    void CreateFieldByType(EFieldType CurrentType, int32 Column, int32 Row, int32 Length);

    UFUNCTION()
    bool CreatNoneBasicFieldTile(EFieldType CurrentType, int32 Column, int32 Row, int32 LengthOfField);

    UFUNCTION()
    void CreateEmpty(int32 Column, int32 Row, int32 Length);
    UFUNCTION()
    void CreateBasic(int32 Column, int32 Row, int32 Length);
    UFUNCTION()
    void CreatHill(int32 Column, int32 Row, int32 Length, int32 StairLength);
    UFUNCTION()
    void CreatWaterSide(int32 Column, int32 Row, int32 Length);
    UFUNCTION()
    void CreateAsphalt(int32 Column, int32 Row, int32 Length);
    UFUNCTION()
    void CreateCave(int32 Column, int32 Row, int32 Length);

    UFUNCTION()
    void SetTileIfPossible(class UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileToSet, bool bEmptyOnly = true);

    // Creat Feature
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<class UPaperSpriteComponent>> FeatureSpriteComponentArr;

    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;

    UFUNCTION()
    void PlaceSpritesOnTileMap(UPaperTileMap* UPaperTileMap, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<struct FCCFeatureInfo>& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree = true);
};

