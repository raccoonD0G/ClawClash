// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCStageMap.h"
#include "CCSpritePlacer.generated.h"

class UPaperTileMapComponent;
class UPaperSprite;
class UPaperSpriteComponent;
struct FCCFeatureInfo;
struct FCCFeatureInfoArrContainer;

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCSpritePlacer : public UObject
{
	GENERATED_BODY()
	
// Layer Section
protected:
    float BackgroundY;
    float PlayerY;
    float FieldTileY;
    float CurrentBeforePlayerFeatureY;
    float CurrentAfterPlayerFeatureY;

// TileMap Info Section
protected:
    int32 TileMapWidth;
    int32 TileMapHeight;
    float TileWidth;
    float TileHeight;

// Sprite Section
public:
    void InitializeSprite(UPaperTileMapComponent* TileMapComponent, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight);

protected:
    TArray<FVector> PlaceSprites(UPaperTileMapComponent* TileMapComponent, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<FCCFeatureInfo>& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree = true, int32 MinSpriteNum = 0, int32 MaxSpriteNum = 100);
    float CalculateYPos(bool bIsBeforePlayer);
    
    FVector2D GetTileSize(UPaperTileMapComponent* TileMapComponent) const;
    FVector2D CalculateEndLocalPos(UPaperTileMapComponent* TileMapComponent, int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos) const;
    void CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite);
    FVector CalculateLocalPos(UPaperTileMapComponent* TileMapComponent, float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite);

    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;
    UPROPERTY()
    TArray<TObjectPtr<UPaperSpriteComponent>> FeatureSpriteComponentArr;
    UPROPERTY()
    TArray<FVector> TreePosArr;

    TArray<FVector> CreateSpriteByType(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreatWaterSideSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreateAsphaltSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreateCaveSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreatHillSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreateRaccoonHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
    TArray<FVector> CreateDogHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
};
