// Fill out your copyright notice in the Description page of Project Settings.


#include "CCField.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapLineParts.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"

UCCField::UCCField()
{
	FieldType = EFieldType::BasicField;
	TileMapPos = FIntVector2(0, 0);
	Length = 0;
}


void UCCField::AddStartPos(int32 AddedNum)
{
	TileMapPos.X += AddedNum;
}

EFieldType UCCField::GetFieldType()
{
	return FieldType;
}

void UCCField::Init(ACCTileMapActor* NewOwningTileMap, FIntVector2 NewTileMapPos, int32 NewLength, EFieldType NewFieldType)
{
	UCCTileMapParts::Init(NewTileMapPos);
	FieldType = NewFieldType;
	Length = NewLength;
	OwningTileMap = NewOwningTileMap;
}

void UCCField::CreateTile()
{
    
}

void UCCField::CreateSprite()
{

}


void UCCField::CreatNoneBasicFieldTile(EFieldType CurrentType)
{
    OwningTileMap->SetTileIfPossible(TileMapPos.X, TileMapPos.Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(CurrentType)->LeftTile, false);
    for (int32 i = TileMapPos.X + 1; i < TileMapPos.X + Length - 1; i++)
    {
        OwningTileMap->SetTileIfPossible(i, TileMapPos.Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(CurrentType)->MiddleTile);
    }
    OwningTileMap->SetTileIfPossible(TileMapPos.X + Length - 1, TileMapPos.Y, 0, UCCStageMapManager::GetInstance()->TileSetPerFieldDic.Find(CurrentType)->RightTile, false);
    OwningTileMap->SetupTileColliders(TileMapPos.X, TileMapPos.Y, Length, FieldType);
}