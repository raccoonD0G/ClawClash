// Fill out your copyright notice in the Description page of Project Settings.


#include "CCRoom.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "CCTileMapParts.h"

const UCCPlatform* UCCRoom::GetPlatform() const
{
    return Platform;
}

void UCCRoom::Init(FIntVector2 NewTileMapPos, int32 InWidth, int32 InHeight)
{
    UCCTileMapParts::Init(NewTileMapPos);
    Width = InWidth;
    Height = InHeight;
} 

FIntVector2 UCCRoom::GetCenter() const
{
    return FIntVector2(TileMapPos.X + Width / 2.0f, TileMapPos.Y + Height / 2.0f);
}

UCCPlatform* UCCRoom::GeneratePlatform()
{
    int32 FloorStartX = FMath::RandRange(TileMapPos.X, TileMapPos.X + Width - UCCManagers::GetInstance()->GetStageMapManager()->MinFloorLength);
    int32 FloorStartY = FMath::RandRange(TileMapPos.Y + UCCManagers::GetInstance()->GetStageMapManager()->MinFloorHeight, TileMapPos.Y + Height - 1);
    Platform = NewObject<UCCPlatform>();
    int32 FloorLength = FMath::RandRange(UCCManagers::GetInstance()->GetStageMapManager()->MinFloorLength, Width - FloorStartX + TileMapPos.X);
    Platform->Init(FIntVector2(FloorStartX, FloorStartY), FloorLength, false);
    return Platform;
}
