// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCTilePlacer.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCTilePlacer : public UObject
{
	GENERATED_BODY()
protected:
	void InitializeTileMap(class UPaperTileMapComponent* TileMapComponent, class UPaperTileSet* TileSet, int32 Rows, int32 Columns, float TileWidth, float TileHeight);

public:
	void SetTileIfPossible(class UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, struct FPaperTileInfo TileInfo, bool bEmptyOnly);

};
