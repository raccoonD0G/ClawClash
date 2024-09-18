// Fill out your copyright notice in the Description page of Project Settings.


#include "CCField.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapLineParts.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"

UCCField::UCCField()
{
	FieldType = EFieldType::NoneField;
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
    switch (FieldType)
    {
    case EFieldType::WatersideField:
        CreatWaterSide();
        break;
    case EFieldType::AsphaltField:
        CreateAsphalt();
        break;
    case EFieldType::CaveField:
        CreateCave();
        break;
    case EFieldType::HillField:
        CreatHill();
        break;
    case EFieldType::RaccoonHouseField:
        CreateRaccoonHouse();
        break;
    case EFieldType::DogHouseField:
        CreateDogHouse();
        break;
    default:
        break;
    }
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


void UCCField::CreatHill()
{
    CreatNoneBasicFieldTile(EFieldType::HillField);
}

void UCCField::CreateRaccoonHouse()
{
    CreatNoneBasicFieldTile(EFieldType::RaccoonHouseField);
}

void UCCField::CreateDogHouse()
{
    CreatNoneBasicFieldTile(EFieldType::DogHouseField);
}

void UCCField::CreatWaterSide()
{
    CreatNoneBasicFieldTile(EFieldType::WatersideField);
}

void UCCField::CreateCave()
{
    CreatNoneBasicFieldTile(EFieldType::CaveField);
    FSpawnableField NewField;
    NewField.SpawnableType = ESpawnableType::Rat;
    NewField.LeftEnd = FVector(TileMapPos.X * UCCStageMapManager::GetStageMap()->GetTileWidth(), 0, -TileMapPos.Y * UCCStageMapManager::GetStageMap()->GetTileHeight() + UCCStageMapManager::GetStageMap()->GetTileHeight() / 2);
    NewField.RightEnd = FVector((TileMapPos.X + Length - 1) * UCCStageMapManager::GetStageMap()->GetTileWidth(), 0, -TileMapPos.Y * UCCStageMapManager::GetStageMap()->GetTileHeight() + UCCStageMapManager::GetStageMap()->GetTileHeight() / 2);
    NewField.MaxCharacterNum = Length;
    UCCSpawnManager::GetInstance()->SpawnFieldMap.Find(EFieldType::CaveField)->SpawnableFieldArr.Add(NewField);
}

void UCCField::CreateAsphalt()
{
    CreatNoneBasicFieldTile(EFieldType::AsphaltField);
}