// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "ClawClash/TileMap/CCStageMap.h"
#include "CCTileMapActor.generated.h"

class UPaperTileMap;

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


// Layer Section
protected:
    float BackgroundY;
    float PlayerY;
    float FieldTileY;

// Init Section
protected:
    void InitializeTileMap();
    void InitializeBackground();
    void PlaceFieldSprites();

// Placer Section
protected:
    UPROPERTY()
    TObjectPtr<class UCCTilePlacer> TilePlacer;

    UPROPERTY()
    TObjectPtr<class UCCSpritePlacer> SpritePlacer;

// FieldTile Section
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> FieldTileMapComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> FieldTileSet;

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
};

