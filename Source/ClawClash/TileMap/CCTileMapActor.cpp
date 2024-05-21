// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"

#include "PaperTileMapComponent.h"
#include "PaperSprite.h"
#include "GameFramework/Character.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"
#include "CCBoxQuadTreeNode.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/CCGameInstance.h"
#include "ClawClash/Managers/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"

#include "CCFloor.h"
#include "CCPlatform.h"
#include "CCField.h"
#include "ClawClash/TileMap/CCStageMap.h"


ACCTileMapActor::ACCTileMapActor()
{
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    BackgroundComponent->SetupAttachment(RootComponent);
    CurrentBeforePlayerFeatureY = 0.1;
    CurrentAfterPlayerFeatureY = 0.1;
}

void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Player
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // Initialize TileMap
    FieldTileMapComponent->CreateNewTileMap(UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth, UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight, UCCManagers::GetInstance()->GetStageMapManager()->TileWidth, UCCManagers::GetInstance()->GetStageMapManager()->TileHeight, 1.0f, true);
    FieldTileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
    FieldTileMapComponent->SetRelativeLocation(FVector(0, FieldTileY, 0));

    RootNode = NewObject<UCCBoxQuadTreeNode>(this);
    RootNode->Initialize(FVector2D(0, -UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight * 512), FVector2D(UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth * 512, 0), 10);

    // Initialize Background
    if (BackGroundSprite != NULL)
    {
        float TileMapWidthInPixels = UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth * 512;
        float TileMapHeightInPixels = UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight * 512;

        BackgroundComponent->SetSprite(BackGroundSprite);
        FVector2D OriginalSize = BackgroundComponent->GetSprite()->GetSourceSize();
        BackgroundComponent->SetRelativeLocation(FVector(Player->GetActorLocation().X, BackgroundY - FieldTileY, Player->GetActorLocation().Z));
        BackgroundComponent->SetRelativeScale3D(FVector(TileMapWidthInPixels / (2.0 * OriginalSize.X), 1.0f, TileMapWidthInPixels / (2.0 * OriginalSize.X)));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }

    if (FieldTileSet)
    {
        UCCManagers::GetInstance()->GetStageMapManager()->CreateStageMap();

        // Initialize Tiles
        int32 TileTypeEnumLength = UCCManagers::GetInstance()->GetEnumLength(StaticEnum<ETileType>());

        for (int32 i = 0; i < TileTypeEnumLength; i++)
        {
            FPaperTileInfo NewTileInfo;
            NewTileInfo.TileSet = FieldTileSet;
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
        
        // Fill Empty Tile
        for (int32 Row = 0; Row < UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight; Row ++)
        {
            for (int32 Column = 0; Column < UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth; Column++)
            {
                SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, *(TileInfoPerTileDic.Find(ETileType::None)), false);
            }
        }

        int32 FloorNum = 0;
        for (int32 Row = (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) - 1; Row <= UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight; Row += UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor)
        {
            for (TObjectPtr<UCCPlatform> Platform : UCCManagers::GetInstance()->GetStageMapManager()->FloorArr[FloorNum]->PlatformArr)
            {
                //PlaceSpritesOnTileMap(FVector2D(Platform->GetStartPos(), Row), Platform->GetLength(), 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false, 4, 5);
                for (TObjectPtr<UCCField> Field : Platform->FieldsInPlatformArr)
                {
                    CreateFieldByType(Field->GetFieldType(), Platform->GetStartPos() + Field->GetStartPos(), Row, Field->GetLength());
                }
            }
            FloorNum++;
        }

        FloorNum = 0;
        for (int32 Row = (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) - 1; Row <= UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight; Row += UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor)
        {
            for (int32 i = 0; i < UCCManagers::GetInstance()->GetStageMapManager()->FloorArr[FloorNum]->PlatformArr.Num() - 1; i++)
            {
                TObjectPtr<UCCPlatform> Platform = UCCManagers::GetInstance()->GetStageMapManager()->FloorArr[FloorNum]->PlatformArr[i];
                TObjectPtr<UCCPlatform> NextPlatform = UCCManagers::GetInstance()->GetStageMapManager()->FloorArr[FloorNum]->PlatformArr[i + 1];
                CreateEmpty(Platform->GetStartPos() + Platform->GetLength(), Row, NextPlatform->GetStartPos() - Platform->GetStartPos() + Platform->GetLength());
            }
            FloorNum++;
        }

        for (int32 Row = (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) - 1; Row <= UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight; Row += UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor)
        {
            for (int32 Column = 0; Column < UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth; Column++)
            {   
                if (FieldTileMapComponent->GetTile(Column, Row, 0).GetTileIndex() == (int32)ETileType::None)
                {
                    CreateBasic(Column, Row, 1);
                }
            }
        }


    }

    FieldTileMapComponent->RebuildCollision();
}

// Called every frame
void ACCTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    BackgroundComponent->SetRelativeLocation(FVector(LastBackgroundX + ((Player->GetActorLocation().X - LastPlayerX) / 1.2), BackgroundY - FieldTileY, LastBackgroundZ + ((Player->GetActorLocation().Z - LastPlayerZ) / 1.2)));
    LastPlayerX = Player->GetActorLocation().X;
    LastPlayerZ = Player->GetActorLocation().Z;
    LastBackgroundX = BackgroundComponent->GetRelativeLocation().X;
    LastBackgroundZ = BackgroundComponent->GetRelativeLocation().Z;

}

void ACCTileMapActor::CreateFieldByType(EFieldType CurrentType, int32 Column, int32 Row, int32 Length)
{
    
    switch (CurrentType)
    {
    case EFieldType::WatersideField:
        CreatWaterSide(Column, Row, Length);
        break;
    case EFieldType::AsphaltField:
        CreateAsphalt(Column, Row, Length);
        break;
    case EFieldType::CaveField:
        CreateCave(Column, Row, Length);
        break;
    case EFieldType::HillField:
        CreatHill(Column, Row, Length, 4);
        break;
    case EFieldType::RaccoonHouseField:
        CreateRaccoonHouse(Column, Row, Length);
        break;
    case EFieldType::DogHouseField:
        CreateDogHouse(Column, Row, Length);
        break;
    default:
        break;
    }
}

void ACCTileMapActor::CreatHill(int32 Column, int32 Row, int32 Length, int32 StairLength)
{
    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->LeftTile);
    for (int32 i = Column + 1; i < Column + Length - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::HillSpace)));
        SetTileIfPossible(FieldTileMapComponent, i, Row + UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor / 4, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + 1 - StairLength; i < Column + 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) / 4 * 2, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + Length; i < Column + Length + StairLength; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) / 4 * 2, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + 1 - StairLength * 2; i < Column + 1 - StairLength; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) / 4 * 3, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    for (int32 i = Column + Length + StairLength; i < Column + Length + StairLength * 2; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight / UCCManagers::GetInstance()->GetStageMapManager()->NumOfFloor) / 4 * 3, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->MiddleTile);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + Length - 1, Row, 0, TileSetPerFieldDic.Find(EFieldType::HillField)->RightTile);
}

void ACCTileMapActor::CreateEmpty(int32 Column, int32 Row, int32 Length)
{
    for (int32 i = 0; i < Length; i++) SetTileIfPossible(FieldTileMapComponent, Column + i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::Empty)));
}

FVector ACCTileMapActor::ConvertTileToWorld(int32 Column, int32 Row) const
{
    float TileWidth = FieldTileMapComponent->TileMap->TileWidth;
    float TileHeight = FieldTileMapComponent->TileMap->TileHeight;

    FVector2D TilePosition2D(Column * TileWidth, Row * TileHeight);

    FVector TileMapLocation = FieldTileMapComponent->GetComponentLocation();

    FRotator TileMapRotation = FieldTileMapComponent->GetComponentRotation();

    FVector LocalTilePosition(TilePosition2D.X, TilePosition2D.Y, 0.0f);
    FVector WorldTilePosition = TileMapLocation + TileMapRotation.RotateVector(LocalTilePosition);

    return WorldTilePosition;
}

void ACCTileMapActor::CreateBasic(int32 Column, int32 Row, int32 Length)
{
    for (int32 i = 0; i < Length; i++) SetTileIfPossible(FieldTileMapComponent, Column + i, Row, 0, *(TileInfoPerTileDic.Find(ETileType::Basic)));
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::GrassFeature)->FeatureInfoArr, false, true);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.2, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::PlantFeature)->FeatureInfoArr, true, true);
}

void ACCTileMapActor::CreateRaccoonHouse(int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(EFieldType::RaccoonHouseField, Column, Row, Length)) return;

    FCCFieldWorldInfo RaccoonField;
    RaccoonField.FieldType = EFieldType::RaccoonHouseField;
    RaccoonField.MiddlePos = ConvertTileToWorld(Column, Row + Length / 2);
    RaccoonField.LeftXPos = ConvertTileToWorld(Column, Row).X;
    RaccoonField.RightXPos = ConvertTileToWorld(Column, Row + Length).X;
    FieldWorldInfoArr.Add(RaccoonField);

    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RacconHouseFeature)->FeatureInfoArr, true, true, false, 1, 1);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    TreePosArr.Append(PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, FMath::RandRange(5.0, 10.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false));
}

void ACCTileMapActor::CreateDogHouse(int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(EFieldType::DogHouseField, Column, Row, Length)) return;

    FCCFieldWorldInfo DogHouseField;
    DogHouseField.FieldType = EFieldType::DogHouseField;
    DogHouseField.MiddlePos = ConvertTileToWorld(Column, Row + Length / 2);
    DogHouseField.LeftXPos = ConvertTileToWorld(Column, Row).X;
    DogHouseField.RightXPos = ConvertTileToWorld(Column, Row + Length).X;
    FieldWorldInfoArr.Add(DogHouseField);

    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::DogHouseFeature)->FeatureInfoArr, true, true, false, 1, 1);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    TreePosArr.Append(PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, FMath::RandRange(5.0, 10.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false));
}

void ACCTileMapActor::CreatWaterSide(int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(EFieldType::WatersideField, Column, Row, Length)) return;

    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::WaterFeature)->FeatureInfoArr, false, true);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::WeedFeature)->FeatureInfoArr, true, true);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::StoneFeature)->FeatureInfoArr, true, true);
    TreePosArr.Append(PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, FMath::RandRange(5.0, 10.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false));
}

void ACCTileMapActor::CreateCave(int32 Column, int32 Row, int32 Length)
{
    if (!CreatNoneBasicFieldTile(EFieldType::CaveField, Column, Row, Length)) return;

    FCCFieldWorldInfo RatCaveField;
    RatCaveField.FieldType = EFieldType::CaveField;
    RatCaveField.MiddlePos = ConvertTileToWorld(Column, Row + Length / 2);
    RatCaveField.LeftXPos = ConvertTileToWorld(Column, Row).X;
    RatCaveField.RightXPos = ConvertTileToWorld(Column, Row + Length).X;
    FieldWorldInfoArr.Add(RatCaveField);

    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RatCaveFeature)->FeatureInfoArr, true, true, false, 1, 4);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    TreePosArr.Append(PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, FMath::RandRange(5.0, 10.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false));
}

void ACCTileMapActor::CreateAsphalt(int32 Column, int32 Row, int32 Length)
{
    if(!CreatNoneBasicFieldTile(EFieldType::AsphaltField, Column, Row, Length)) return;
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::BuildingFeature)->FeatureInfoArr, true, false, true);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::CarFeature)->FeatureInfoArr, true, true);
    PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::LightFeature)->FeatureInfoArr, true, true);
    TreePosArr.Append(PlaceSpritesOnTileMap(FVector2D(Column, Row), Length, FMath::RandRange(5.0, 10.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, true, true, false));
}

TArray<FVector> ACCTileMapActor::PlaceSpritesOnTileMap(FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<FCCFeatureInfo>& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    OffsetTiles--;

    int32 SpriteNum = 0;
    TArray<FVector> PlacedSpritePositions;

    if (!FieldTileMapComponent->TileMap)
    {
        return PlacedSpritePositions;
    }

    FVector2D TileSize = GetTileSize();
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos = CalculateEndLocalPos(OffsetTiles, TileSize, StartLocalPos);

    FTransform ActorTransform = GetActorTransform();
    FVector StartPos = ActorTransform.TransformPosition(FVector(StartLocalPos, 0.0f));
    FVector EndPos = ActorTransform.TransformPosition(FVector(EndLocalPos, 0.0f));

    do
    {
        for (float i = StartPos.X; i <= EndPos.X; i += TileSize.X * TileInterval)
        {
            PlaceSpriteAtPosition(i, StartPos, TileSize, TileInterval, FeatureInfoArr, bIsBeforePlayer, bAllowOverlap, bAddToCollisionTree, SpriteNum, MaxSpriteNum, PlacedSpritePositions);
        }
    } while (SpriteNum < MinSpriteNum);

    return PlacedSpritePositions;
}

FVector2D ACCTileMapActor::GetTileSize()
{
    return FVector2D(FieldTileMapComponent->TileMap->TileWidth, FieldTileMapComponent->TileMap->TileHeight);
}

FVector2D ACCTileMapActor::CalculateEndLocalPos(int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos)
{
    if (OffsetTiles * TileSize.X < FieldTileMapComponent->TileMap->TileWidth * UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth - StartLocalPos.X)
        return StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
    else
        return StartLocalPos + FVector2D(FieldTileMapComponent->TileMap->TileWidth * (UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth - 1) - StartLocalPos.X, 0);
}

void ACCTileMapActor::PlaceSpriteAtPosition(float XPos, FVector StartPos, FVector2D TileSize, float TileInterval, const TArray<FCCFeatureInfo>& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32& SpriteNum, int32 MaxSpriteNum, TArray<FVector>& PlacedSpritePositions)
{
    int32 Index = UCCManagers::GetInstance()->GetRandomIndexByProbability(FeatureInfoArr);
    if (Index != 0)
    {
        Index--;
        float YPos = CalculateYPos(bIsBeforePlayer);
        FVector LocalPos = CalculateLocalPos(XPos, StartPos, TileSize, FeatureInfoArr[Index].FeatureSprite);
        LocalPos.Y = YPos;

        FBox2D BoxForSprite(FVector2D(LocalPos.X, LocalPos.Z), FVector2D(LocalPos.X + FeatureInfoArr[Index].FeatureSprite->GetSourceSize().X, LocalPos.Z + FeatureInfoArr[Index].FeatureSprite->GetSourceSize().Y));

        if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
        {
            if (SpriteNum >= MaxSpriteNum) return;
            SpriteNum++;
            CreateAndAttachSpriteComponent(FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite);
            LocalPos.Y = FieldTileY;
            PlacedSpritePositions.Add(LocalPos);
        }
    }
}

float ACCTileMapActor::CalculateYPos(bool bIsBeforePlayer)
{
    float YPos;
    if (bIsBeforePlayer)
    {
        YPos = BackgroundY + CurrentBeforePlayerFeatureY;
        CurrentBeforePlayerFeatureY += 0.1f;
    }
    else
    {
        YPos = PlayerY + CurrentAfterPlayerFeatureY;
        CurrentAfterPlayerFeatureY += 0.1f;
    }
    YPos -= FieldTileY;
    return YPos;
}

FVector ACCTileMapActor::CalculateLocalPos(float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite)
{
    return FVector(XPos, StartPos.Y, FieldTileMapComponent->GetRelativeLocation().Y + -StartPos.Y + TileSize.Y + (FeatureSprite->GetSourceSize().Y - 512) / 2);
}

void ACCTileMapActor::CreateAndAttachSpriteComponent(UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, const FBox2D& BoxForSprite)
{
    TObjectPtr<UPaperSpriteComponent> NewSprite = NewObject<UPaperSpriteComponent>(this);
    if (NewSprite)
    {
        NewSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        NewSprite->RegisterComponent();
        NewSprite->SetVisibility(true);
        NewSprite->SetSprite(FeatureSprite);
        NewSprite->SetRelativeLocation(LocalPos);
        NewSprite->SetCollisionProfileName(TEXT("NoCollision"));
        NewSprite->SetMaterial(0, DefaultSpriteMaterial);
        NewSprite->bReceivesDecals = false;
        NewSprite->CastShadow = false;

        bool bShouldFlip = UKismetMathLibrary::RandomBool();
        float NewScaleX = bShouldFlip ? -1.0f : 1.0f;
        NewSprite->SetRelativeScale3D(FVector(NewScaleX, 1, 1));

        if (bAddToCollisionTree) RootNode->Insert(BoxForSprite);
        FeatureSpriteComponentArr.Add(NewSprite);
    }
}

bool ACCTileMapActor::CreatNoneBasicFieldTile(EFieldType CurrentType, int32 Column, int32 Row, int32 LengthOfField)
{
    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileSetPerFieldDic.Find(CurrentType)->LeftTile);
    for (int32 i = Column + 1; i < Column + LengthOfField - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileSetPerFieldDic.Find(CurrentType)->MiddleTile);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfField - 1, Row, 0, TileSetPerFieldDic.Find(CurrentType)->RightTile);

    return true;
}

void ACCTileMapActor::SetTileIfPossible(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    switch (Layer)
    {
    case 0:
        if (Column >= 0 && Column < TileMapComponent->TileMap->MapWidth && Row >= 0 && Row < TileMapComponent->TileMap->MapHeight)
            if (bEmptyOnly == false || TileMapComponent->GetTile(Column, Row, 0).GetTileIndex() == (int32)ETileType::None)
            {
                TileMapComponent->SetTile(Column, Row, 0, TileInfo);
            }
        break;
    default:
        break;
    }
}
