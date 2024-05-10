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

USTRUCT()
struct FCCFeatureInfo
{
    GENERATED_BODY()

    int32 FeatureNum;

    UPROPERTY()
    TArray<TObjectPtr<class UPaperSprite>> SpriteArr;

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

    TArray<class UPaperSprite*> GetAllSpritesFromFolder(const FString& FolderPath);

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
    UPROPERTY()
    FCCFeatureInfo WeedFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo RockFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo BuildingFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo CarFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo LightFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo PlantFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo StoneFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo GrassFeatureInfo;

    UPROPERTY()
    FCCFeatureInfo WaterFeatureInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    TObjectPtr<class UMaterialInterface> DefaultSpriteMaterial;

    // TileMap Section
protected:
    // Creat TileMap
    const int32 TileMapWidth = 128;
    const int32 TileMapHeight = 64;
    const int32 NumOfFloor = 4;

    // Create Field
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> FieldTileSet;

    UPROPERTY()
    TArray<float> FieldRatio;

    UPROPERTY()
    TArray<struct FCCFieldInfo> FieldInfoArr;

    UPROPERTY()
    TArray<struct FPaperTileInfo> TileInfoArr;

    UFUNCTION()
    void CreateFieldByType(EFieldType CurrentType, int32 Column, int32 Row);
    UFUNCTION()
    void CreatBasic(int32 Column, int32 Row);
    UFUNCTION()
    void CreatHill(int32 Column, int32 Row, int32 StairLength);
    UFUNCTION()
    void CreatWaterSide(int32 Column, int32 Row);
    UFUNCTION()
    void CreateAsphalt(int32 Column, int32 Row);
    UFUNCTION()
    void CreateCave(int32 Column, int32 Row);

    void SetTileIfPossible(TObjectPtr<class UPaperTileMapComponent> TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileToSet, bool bEmptyOnly = true);

    UFUNCTION()
    bool CheckAllEmpty(int32 Column, int32 Row, int32 Length);

    // Creat Feature
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TArray<TObjectPtr<class UPaperSpriteComponent>> FeatureSpriteComponentArr;

    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;

    void PlaceSpritesOnTileMap(TObjectPtr<class UPaperTileMap> UPaperTileMap, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, TArray<TObjectPtr<class UPaperSprite>> SpriteToPlace, TArray<float> RatioArr, float YPos = 0.0, bool bAllowOverlap = false, bool bAddToCollisionTree = true);
};

