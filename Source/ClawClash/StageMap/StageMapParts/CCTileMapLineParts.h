// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCTileMapParts.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "CCTileMapLineParts.generated.h"

class UPaperSprite;
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCTileMapLineParts : public UCCTileMapParts
{
	GENERATED_BODY()
	
protected:
	int32 Length;
	void PlaceSprites(float TileInterval, const FCCFeatureInfoArrContainer& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree = true, int32 MinSpriteNum = 0, int32 MaxSpriteNum = 100);
	FBox2D GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos);
	float CalculateYPos(bool bIsBeforePlayer);
	FVector2D GetTileSize() const;
	FVector2D CalculateEndLocalPos(int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos) const;
	void CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer);
	FVector CalculateLocalPos(float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite, bool bIsBeforePlayer);

public:
	FIntVector2 GetStartPos() const;
	int32 GetLength() const;
	FIntVector2 GetEndPos() const;
	
};
