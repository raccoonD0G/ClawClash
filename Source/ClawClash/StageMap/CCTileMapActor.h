// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapActor.generated.h"

class UPaperTileMap;
class UCCPlatform;
class UCCRoom;

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

public:
    virtual void Tick(float DeltaTime) override;

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
    int32 GetTileMapWidth();
    int32 GetTileMapHeight();
    int32 GetTileWidth();
    int32 GetTileHeight();
    int32 GetMinRoomHeight();
    int32 GetMinRoomWidth();

// Init Section
protected:
    void InitializeBackground();

// FieldTile Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> FieldTileSet;

// Create Map Section
public:
    const TArray<UCCPlatform*>& GetPlatformArr();

protected:
    UPROPERTY()
    TArray<TObjectPtr<UCCPlatform>> PlatformArr;

    void SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth);
    void GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight);
    float CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2);
    void GenerateMST();
    void CreatePlatformsAlongEdge(const FPlatformEdge& Edge);
    void InitializeTileMap(UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight);

// Tile Section
public:
    bool SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly = true);
    
// Background Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UPaperSpriteComponent> BackgroundComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TObjectPtr<class UPaperSprite> BackGroundSprite;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TArray<TObjectPtr<class UPaperSprite>> BackGroundSpriteArr;

    UPROPERTY()
    TObjectPtr<class ACharacter> Player;

    int32 LastPlayerX;
    int32 LastPlayerZ;

    int32 LastBackgroundX;
    int32 LastBackgroundZ;

// Collision Section
public:
    void SetupTileColliders(int32 Column, int32 Row, int32 ColliderLength, EFieldType NewFieldType);

protected:
    UPROPERTY()
    TArray<TObjectPtr<class UCCTileCollider>> ColliderArr;

// Sprite Section
public:
    class UCCBoxQuadTreeNode* GetRootNode();
    void AddSpriteComponentArr(UPaperSpriteComponent* NewSpriteComponent);

    int32 GetBeforePlayerOrder();
    int32 GetAfterPlayerOrder();

    void AddBeforPlayerOrder();
    void AddAfterPlayerOrder();

protected:
    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;
    UPROPERTY()
    TArray<TObjectPtr<UPaperSpriteComponent>> FeatureSpriteComponentArr;

    int32 BeforePlayerOrder;
    int32 AfterPlayerOrder;
};
