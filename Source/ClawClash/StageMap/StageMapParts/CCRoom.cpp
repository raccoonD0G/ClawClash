// Fill out your copyright notice in the Description page of Project Settings.


#include "CCRoom.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "CCTileMapParts.h"

const UCCPlatform* UCCRoom::GetPlatform() const
{
    return Platform;
}

void UCCRoom::Init(ACCTileMapActor* NewOwningTileMap, FIntVector2 NewTileMapPos, int32 InWidth, int32 InHeight)
{
    UCCTileMapParts::Init(NewTileMapPos);
    Width = InWidth;
    Height = InHeight;
    OwningTileMap = NewOwningTileMap;
} 

FIntVector2 UCCRoom::GetCenter() const
{
    return FIntVector2(TileMapPos.X + Width / 2.0f, TileMapPos.Y + Height / 2.0f);
}

UCCPlatform* UCCRoom::GeneratePlatform()
{
    int32 FloorStartX = FMath::RandRange(TileMapPos.X, TileMapPos.X + Width - OwningTileMap->GetMinRoomWidth());
    int32 FloorStartY = FMath::RandRange(TileMapPos.Y + OwningTileMap->GetMinRoomHeight(), TileMapPos.Y + Height - 1);
    Platform = NewObject<UCCPlatform>();
    int32 FloorLength = FMath::RandRange(OwningTileMap->GetMinRoomWidth(), Width - FloorStartX + TileMapPos.X);
    Platform->Init(OwningTileMap, FIntVector2(FloorStartX, FloorStartY), FloorLength);
    return Platform;
}
