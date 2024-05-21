// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCSpritePlacer.generated.h"

class UPaperTileMapComponent;
class UPaperSprite;
class UPaperSpriteComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCSpritePlacer : public UObject
{
	GENERATED_BODY()
	
public:
    TArray<FVector> PlaceSprites(UPaperTileMapComponent* TileMapComponent, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<UPaperSprite*>& SpriteArray, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum);

private:
    FVector2D GetTileSize(UPaperTileMapComponent* TileMapComponent) const;
    FVector2D CalculateEndLocalPos(int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos, int32 MapWidth) const;
    void CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree);

    UPROPERTY()
    TObjectPtr<class UCCBoxQuadTreeNode> RootNode;
    UPROPERTY()
    TArray<TObjectPtr<UPaperSpriteComponent>> FeatureSpriteComponentArr;

    FBox2D GetSpriteComponentBounds(UPaperSpriteComponent* SpriteComponent);
	
};
