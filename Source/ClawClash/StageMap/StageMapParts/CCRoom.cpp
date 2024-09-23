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

void UCCRoom::Init(FIntVector2 NewTileMapPos, int32 InWidth, int32 InHeight, int32 NewMinPlatformLength, int32 NewMinPlatformHeight)
{
    UCCTileMapParts::Init(NewTileMapPos);
    Width = InWidth;
    Height = InHeight;
    MinPlatformLength = NewMinPlatformLength;
    MinPlatformHeight = NewMinPlatformHeight;
}

FIntVector2 UCCRoom::GetCenter() const
{
    return FIntVector2(TileMapPos.X + Width / 2.0f, TileMapPos.Y + Height / 2.0f);
}

UCCPlatform* UCCRoom::GeneratePlatform()
{
    int32 FloorStartX = FMath::RandRange(TileMapPos.X, TileMapPos.X + Width - MinPlatformLength);
    int32 FloorStartY = FMath::RandRange(TileMapPos.Y + MinPlatformHeight, TileMapPos.Y + Height - 1);
    Platform = NewObject<UCCPlatform>();
    int32 FloorLength = FMath::RandRange(MinPlatformLength, Width - FloorStartX + TileMapPos.X);
    Platform->Init(FIntVector2(FloorStartX, FloorStartY), FloorLength);
    return Platform;
}
