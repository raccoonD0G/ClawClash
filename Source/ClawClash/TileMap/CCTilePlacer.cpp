// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTilePlacer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"

void UCCTilePlacer::InitializeTileMap(UPaperTileMapComponent* TileMapComponent, UPaperTileSet* TileSet, int32 Rows, int32 Columns, float TileWidth, float TileHeight)
{
    if (TileMapComponent && TileSet)
    {
        TileMapComponent->CreateNewTileMap(Columns, Rows, TileWidth, TileHeight, 1.0f, true);
        TileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);

        for (int32 Row = 0; Row < Rows; ++Row)
        {
            for (int32 Column = 0; Column < Columns; ++Column)
            {
                FPaperTileInfo TileInfo;
                TileInfo.TileSet = TileSet;
                TileInfo.PackedTileIndex = 0;
                SetTileIfPossible(TileMapComponent, Column, Row, 0, TileInfo, false);
            }
        }
    }
}

void UCCTilePlacer::SetTileIfPossible(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    if (TileMapComponent)
    {
        if (Column >= 0 && Column < TileMapComponent->TileMap->MapWidth && Row >= 0 && Row < TileMapComponent->TileMap->MapHeight)
        {
            if (!bEmptyOnly || TileMapComponent->GetTile(Column, Row, Layer).GetTileIndex() == -1)
            {
                TileMapComponent->SetTile(Column, Row, Layer, TileInfo);
            }
        }
    }
}
