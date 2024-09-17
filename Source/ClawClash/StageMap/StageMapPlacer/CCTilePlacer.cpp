// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTilePlacer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/StageMapParts/CCField.h"
#include "ClawClash/StageMap/StageMapParts/CCRoom.h"
#include "ClawClash/StageMap/CCStageMap.h"
#include "ClawClash/StageMap/CCTileCollider.h"


void UCCTilePlacer::InitializeTileMap(UPaperTileMapComponent* TileMapComponent, UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight)
{
    TileMapHeight = Rows;
    TileMapWidth = Columns;
    TileWidth = NewTileWidth;
    TileHeight = NewTileHeight;

    if (TileMapComponent && TileSet)
    {
        TileMapComponent->CreateNewTileMap(Columns, Rows, TileWidth, TileHeight, 1.0f, true);
        TileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
        TileMapComponent->SetRelativeLocation(FVector(0, FieldTileY, 0));

        for (int32 Row = 0; Row < Rows; ++Row)
        {
            for (int32 Column = 0; Column < Columns; ++Column)
            {
                FPaperTileInfo TileInfo;
                TileInfo.TileSet = TileSet;
                TileInfo.PackedTileIndex = (int32)ETileType::None;
                SetTileIfPossible(TileMapComponent, Column, Row, 0, TileInfo, false);
            }
        }
    }
}

void UCCTilePlacer::CreatAllField(UPaperTileMapComponent* TileMapComponent)
{
    for (UCCPlatform* Platform : UCCStageMapManager::GetInstance()->GetStageMap().GetPlatformArr())
    {
        for (UCCField* Field : Platform->GetFieldArr())
        {
            CreateFieldByType(TileMapComponent, Field->GetFieldType(), Field->GetTileMapPos().X, Field->GetTileMapPos().Y, Field->GetLength());
        }
    }
}

FVector UCCTilePlacer::GetWorldLocationFromTile(UPaperTileMapComponent* TileMapComponent, int32 Row, int32 Column)
{
    if (!TileMapComponent || !TileMapComponent->TileMap)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid TileMapComponent or TileMap"));
        return FVector::ZeroVector;
    }

    const FVector ComponentLocation = TileMapComponent->GetComponentLocation();
    const FRotator ComponentRotation = TileMapComponent->GetComponentRotation();
    const FTransform ComponentTransform = TileMapComponent->GetComponentTransform();

    FVector2D TileLocalPosition = FVector2D(Column * TileMapComponent->TileMap->TileWidth, Row * TileMapComponent->TileMap->TileHeight);

    FVector TileWorldPosition = ComponentTransform.TransformPosition(FVector(TileLocalPosition, 0.0f));

    return TileWorldPosition;
}

void UCCTilePlacer::FillInBasic(UPaperTileMapComponent* TileMapComponent)
{
    for (UCCPlatform* Platform : UCCStageMapManager::GetInstance()->GetStageMap().GetPlatformArr())
    {
        CreateBasic(TileMapComponent, Platform->GetStartPos().X, Platform->GetStartPos().Y, Platform->GetLength());
    }
}

void UCCTilePlacer::SetupTileColliders(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row)
{
    int32 TileSize = UCCStageMapManager::GetInstance()->TileHeight;

    UCCTileCollider* CustomCollider = NewObject<UCCTileCollider>(TileMapComponent->GetOwner());
    ColliderArr.Add(CustomCollider);
    CustomCollider->AttachToComponent(TileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CustomCollider->SetBoxExtent(FVector(TileSize / 2, 1000.0f, TileSize / 2));
    CustomCollider->SetRelativeLocation(FVector(Column * TileSize, 0, -Row * TileSize));

    CustomCollider->RegisterComponent();
}


void UCCTilePlacer::SetTileIfPossible(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    if (TileMapComponent)
    {
        if (Column >= 0 && Column < TileMapComponent->TileMap->MapWidth && Row >= 0 && Row < TileMapComponent->TileMap->MapHeight)
        {
            if (!bEmptyOnly || TileMapComponent->GetTile(Column, Row, Layer).GetTileIndex() == (int32)ETileType::None)
            {
                TileMapComponent->SetTile(Column, Row, Layer, TileInfo);
            }
        }
    }
}

void UCCTilePlacer::CreateFieldByType(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 Length)
{
    switch (CurrentType)
    {
    case EFieldType::WatersideField:
        CreatWaterSide(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::AsphaltField:
        CreateAsphalt(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::CaveField:
        CreateCave(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::HillField:
        CreatHill(TileMapComponent, Column, Row, Length, 4);
        break;
    case EFieldType::RaccoonHouseField:
        CreateRaccoonHouse(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::DogHouseField:
        CreateDogHouse(TileMapComponent, Column, Row, Length);
        break;
    default:
        break;
    }
}

bool UCCTilePlacer::CreatNoneBasicFieldTile(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 LengthOfField)
{
    SetTileIfPossible(TileMapComponent, Column, Row, 0, TileSetPerFieldDic.Find(CurrentType)->LeftTile);
    for (int32 i = Column + 1; i < Column + LengthOfField - 1; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row, 0, TileSetPerFieldDic.Find(CurrentType)->MiddleTile);
        SetupTileColliders(TileMapComponent, i, Row);
    }
    SetTileIfPossible(TileMapComponent, Column + LengthOfField - 1, Row, 0, TileSetPerFieldDic.Find(CurrentType)->RightTile);

    return true;
}

void UCCTilePlacer::InitializeTileSet(UPaperTileSet* NewFieldTileSet)
{
    // Initialize Tiles
    int32 TileTypeEnumLength = UCCUtils::GetEnumLength(StaticEnum<ETileType>());

    for (int32 i = 0; i < TileTypeEnumLength; i++)
    {
        FPaperTileInfo NewTileInfo;
        NewTileInfo.TileSet = NewFieldTileSet;
        NewTileInfo.PackedTileIndex = i;
        TileInfoPerTileDic.Add((ETileType)i, NewTileInfo);
    }

    FCCFieldTileSet WatersideTileSet;
    WatersideTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::WatersideLeft));
    WatersideTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Waterside));
    WatersideTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::WatersideRight));
    TileSetPerFieldDic.Add(EFieldType::WatersideField, WatersideTileSet);

    FCCFieldTileSet AsphaltTileSet;
    AsphaltTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::AsphaltLeft));
    AsphaltTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Asphalt));
    AsphaltTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::AsphaltRight));
    TileSetPerFieldDic.Add(EFieldType::AsphaltField, AsphaltTileSet);

    FCCFieldTileSet CaveTileSet;
    CaveTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::CaveLeft));
    CaveTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Cave));
    CaveTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::CaveRight));
    TileSetPerFieldDic.Add(EFieldType::CaveField, CaveTileSet);

    FCCFieldTileSet HillTileSet;
    HillTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::HillLeft));
    HillTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Hill));
    HillTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::HillRight));
    TileSetPerFieldDic.Add(EFieldType::HillField, HillTileSet);

    FCCFieldTileSet RaccoonHouseTileSet;
    RaccoonHouseTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::CaveLeft));
    RaccoonHouseTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Cave));
    RaccoonHouseTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::CaveRight));
    TileSetPerFieldDic.Add(EFieldType::RaccoonHouseField, RaccoonHouseTileSet);

    FCCFieldTileSet DogHouseTileSet;
    DogHouseTileSet.LeftTile = *(TileInfoPerTileDic.Find(ETileType::CaveLeft));
    DogHouseTileSet.MiddleTile = *(TileInfoPerTileDic.Find(ETileType::Cave));
    DogHouseTileSet.RightTile = *(TileInfoPerTileDic.Find(ETileType::CaveRight));
    TileSetPerFieldDic.Add(EFieldType::DogHouseField, DogHouseTileSet);
}

void UCCTilePlacer::CreatHill(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length, int32 StairLength)
{
    SetTileIfPossible(TileMapComponent, Column, Row, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->LeftTile);
    for (int32 i = Column + 1; i < Column + Length - 1; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::HillSpace)));
        SetTileIfPossible(TileMapComponent, i, Row + TileMapHeight / UCCStageMapManager::GetInstance()->NumOfFloor / 4, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + 1 - StairLength; i < Column + 1; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row + (TileMapHeight / UCCStageMapManager::GetInstance()->NumOfFloor) / 4 * 2, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + Length; i < Column + Length + StairLength; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row + (TileMapHeight / UCCStageMapManager::GetInstance()->NumOfFloor) / 4 * 2, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + 1 - StairLength * 2; i < Column + 1 - StairLength; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row + (TileMapHeight / UCCStageMapManager::GetInstance()->NumOfFloor) / 4 * 3, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + Length + StairLength; i < Column + Length + StairLength * 2; i++)
    {
        SetTileIfPossible(TileMapComponent, i, Row + (TileMapHeight / UCCStageMapManager::GetInstance()->NumOfFloor) / 4 * 3, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    SetTileIfPossible(TileMapComponent, Column + Length - 1, Row, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->RightTile);
}

void UCCTilePlacer::CreateBasic(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    for (int32 i = 0; i < Length; i++)
    {
        SetTileIfPossible(TileMapComponent, Column + i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::Basic)));
        SetupTileColliders(TileMapComponent, Column + i, Row);
    }
}

void UCCTilePlacer::CreateRaccoonHouse(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(TileMapComponent, EFieldType::RaccoonHouseField, Column, Row, Length)) return;
}

void UCCTilePlacer::CreateDogHouse(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(TileMapComponent, EFieldType::DogHouseField, Column, Row, Length)) return;
}

void UCCTilePlacer::CreatWaterSide(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(TileMapComponent, EFieldType::WatersideField, Column, Row, Length)) return;
}

void UCCTilePlacer::CreateCave(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(TileMapComponent, EFieldType::CaveField, Column, Row, Length)) return;
    FSpawnableField NewField;
    NewField.SpawnableType = ESpawnableType::Rat;
    NewField.LeftEnd = FVector(Column * UCCStageMapManager::GetInstance()->TileWidth, 0, -Row * UCCStageMapManager::GetInstance()->TileHeight + UCCStageMapManager::GetInstance()->TileHeight / 2);
    NewField.RightEnd = FVector((Column + Length - 1) * UCCStageMapManager::GetInstance()->TileWidth, 0, -Row * UCCStageMapManager::GetInstance()->TileHeight + UCCStageMapManager::GetInstance()->TileHeight / 2);
    NewField.MaxCharacterNum = Length;
    UCCSpawnManager::GetInstance()->SpawnFieldMap.Find(EFieldType::CaveField)->SpawnableFieldArr.Add(NewField);
}

void UCCTilePlacer::CreateAsphalt(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(TileMapComponent, EFieldType::AsphaltField, Column, Row, Length)) return;
}

void UCCTilePlacer::CreateEmpty(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    for (int32 i = 0; i < Length; i++) SetTileIfPossible(TileMapComponent, Column + i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::Empty)));
}
