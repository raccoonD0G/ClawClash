// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "CCTileMapActor.generated.h"

USTRUCT()
struct FCCFieldInfo
{
    GENERATED_BODY()

    UPROPERTY()
    int32 MinLenght;

    UPROPERTY()
    int32 MaxLength;

    UPROPERTY()
    TArray<float> FeatureRatio;
};

UCLASS()
class CLAWCLASH_API ACCTileMapActor : public AActor
{
	GENERATED_BODY()
	
public:
    ACCTileMapActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

// Util Section
protected:
    UFUNCTION()
    int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);

    int32 GetEnumLength(TObjectPtr<UEnum> TargetEnum);

// Sprite Section
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TObjectPtr<class UPaperSprite> WeedSprite;

// TileMap Section
protected:
    UFUNCTION()
    void CreateFieldByType(EFieldType CurrentType, int32 Column, int32 Row);
    UFUNCTION()
    void CreatHill(int32 Column, int32 Row, int32 StairLength);
    UFUNCTION()
    void CreatWaterSide(int32 Column, int32 Row);
    UFUNCTION()
    void CreateAsphalt(int32 Column, int32 Row);

    void SetTileIfPossible(TObjectPtr<class UPaperTileMapComponent> TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileToSet, bool bEmptyOnly = true);
    UFUNCTION()
    bool CheckAllEmpty(int32 Column, int32 Row, int32 Length);
    UFUNCTION()
    void CreateCave(int32 Column, int32 Row);

    void PlaceSpritesOnTileMap(TObjectPtr<class UPaperTileMap> UPaperTileMap, FVector2D StartingTile, int32 OffsetTiles, TObjectPtr<class UPaperSprite> SpriteToPlace, bool bAllowOverlap = false);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> FieldTileMapComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<class UPaperSpriteComponent>> FeatureSpriteComponentArr;

    // Reference to the TileSet
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> FieldTileSet;

    UPROPERTY()
    TArray<struct FCCFieldInfo> FieldInfoArr;
    UPROPERTY()
    TArray<struct FPaperTileInfo> TileInfoArr;
    UPROPERTY()
    TArray<struct FPaperTileInfo> WaterSideFeatureInfoArr;

    UPROPERTY()
    TArray<float> FieldRatio;

    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;

    int32 TileMapWeidth = 256;
    int32 TileMapHeight = 64;
    int32 NumOfFloor = 4;
};

