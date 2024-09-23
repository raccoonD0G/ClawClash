// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapLineParts.h"
#include "CCTileMapParts.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "CCField.h"
#include "CCPlatform.h"
#include "PaperTileMapComponent.h"
#include "PaperSpriteComponent.h"
#include "ClawClash/StageMap/CCBoxQuadTreeNode.h"
#include <ClawClash/Managers/LayerManager/CCLayerManager.h>

FIntVector2 UCCTileMapLineParts::GetStartPos() const
{
	return GetTileMapPos();
}

int32 UCCTileMapLineParts::GetLength() const
{
	return Length;
}

FIntVector2 UCCTileMapLineParts::GetEndPos() const
{
	return FIntVector2(GetTileMapPos().X + Length, GetTileMapPos().Y);
}