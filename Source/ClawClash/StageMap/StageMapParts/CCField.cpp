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

void UCCField::Init(FIntVector2 NewTileMapPos, int32 NewLength, EFieldType NewFieldType)
{
	UCCTileMapParts::Init(NewTileMapPos);
	FieldType = NewFieldType;
	Length = NewLength;
}