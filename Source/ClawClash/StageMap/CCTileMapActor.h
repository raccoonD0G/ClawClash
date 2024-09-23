// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "CCTileMapActor.generated.h"

class UPaperTileMap;
class UCCField;
class UCCPlatform;
class UCCRoom;
class UPaperSpriteComponent;
class UCCBoxQuadTreeNode;
class UCCTileCollider;
class UPaperTileMapComponent;
struct FCCFeatureInfoArrContainer;
class UPaperSprite;
struct FSpawnableField;
class ACCSpawnerSpawner;

USTRUCT()
struct FPlatformEdge
{
    GENERATED_BODY()
public:

    int32 PlatformIndex1;
    int32 PlatformIndex2;
    float Weight;
    FIntVector2 Pos1;
    FIntVector2 Pos2;

    FPlatformEdge()
    {

    }

    FPlatformEdge(int32 InPlatformIndex1, int32 InPlatformIndex2, float InWeight, FIntVector2 InPos1, FIntVector2 InPos2)
        : PlatformIndex1(InPlatformIndex1), PlatformIndex2(InPlatformIndex2), Weight(InWeight), Pos1(InPos1), Pos2(InPos2)
    {
    }

    bool operator<(const FPlatformEdge& Other) const
    {
        return Weight < Other.Weight;
    }
};

UCLASS()
class CLAWCLASH_API ACCTileMapActor : public AActor
{
    GENERATED_BODY()

public:
    ACCTileMapActor();

protected:
    virtual void BeginPlay() override;

// Info Section
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapWidth = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileMapHeight = 64;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileWidth = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 TileHeight = 512;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 MinRoomHeight = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageMap")
    int32 MinRoomWidth = 11;

public:
    FORCEINLINE int32 GetTileMapWidth() const { return TileMapWidth; }
    FORCEINLINE int32 GetTileMapHeight() const { return TileMapHeight; }
    FORCEINLINE int32 GetTileWidth() const { return TileWidth; }
    FORCEINLINE int32 GetTileHeight() const { return TileHeight; }
    FORCEINLINE int32 GetMinRoomHeight() const { return MinRoomHeight; }
    FORCEINLINE int32 GetMinRoomWidth() const { return MinRoomWidth; }

// FieldTile Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<UPaperTileSet> FieldTileSet;

// Create Map Section
public:
    FORCEINLINE const TArray<UCCField*>& GetFieldArr() const { return FieldArr; }

protected:
    UPROPERTY()
    TArray<TObjectPtr<UCCField>> FieldArr;
    FVector GetWorldSpaceStartPos(UCCField* Field) const;
    FVector GetWorldSpaceEndPos(UCCField* Field) const;

    void SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth) const;
    void GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight) const;
    float CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2) const;
    void GenerateMST(TArray<UCCPlatform*>& PlatformArr) const;
    void CreatePlatformsAlongEdge(TArray<UCCPlatform*>& PlatformArr, const FPlatformEdge& Edge) const;
    void InitializeTileMap(UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight);

// Tile Section
public:
    bool SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly = true);
    void CreatFieldTile(UCCField* Field);

// Collision Section
public:
    void SetupTileColliders(int32 Column, int32 Row, int32 ColliderLength, EFieldType NewFieldType);

protected:
    UPROPERTY()
    TArray<TObjectPtr<UCCTileCollider>> ColliderArr;

// Sprite Section
public:
    void AddSpriteComponentArr(UPaperSpriteComponent* NewSpriteComponent);

    FORCEINLINE int32 GetBeforePlayerOrder() const { return BeforePlayerOrder; }
    FORCEINLINE int32 GetAfterPlayerOrder() const { return AfterPlayerOrder; }

    FORCEINLINE void AddBeforPlayerOrder() { BeforePlayerOrder++; }
    FORCEINLINE void AddAfterPlayerOrder() { AfterPlayerOrder++; }

    void PlaceSpriteEachField(UCCField* Field);
    void PlaceSprites(UCCField* Field, float TileInterval, const FCCFeatureInfoArrContainer& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree = true, int32 MinSpriteNum = 0, int32 MaxSpriteNum = 100);
    
    FBox2D GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos);
    void CreateAndAttachSpriteComponent(UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer);
    FVector CalculateSpriteWorldSpacePos(float XPos, FVector StartPos, UPaperSprite* FeatureSprite, bool bIsBeforePlayer);

protected:
    UPROPERTY()
    TObjectPtr<UCCBoxQuadTreeNode> RootNode;
    UPROPERTY()
    TArray<TObjectPtr<UPaperSpriteComponent>> FeatureSpriteComponentArr;

    int32 BeforePlayerOrder;
    int32 AfterPlayerOrder;

// Spawn Section
protected:
    FSpawnableField ChangeIntoSpawnableField(UCCField* Field, ESpawnableType SpawnableType, int32 MaxCharacterNum);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<ACCSpawnerSpawner> SpawnerSpawner;
};
