// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpritePlacer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"
#include "ClawClash/StageMap/CCBoxQuadTreeNode.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/StageMapParts/CCField.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "PaperTileLayer.h"

UCCSpritePlacer::UCCSpritePlacer()
{
    BeforPlayerOrder = 0;
    AfterPlayerOrder = 0;
}

void UCCSpritePlacer::PlaceSprites(UPaperTileMapComponent* TileMapComponent, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const FCCFeatureInfoArrContainer& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    OffsetTiles--;

    int32 SpriteNum = 0;

    if (!TileMapComponent->TileMap)
    {
        return;
    }

    
    FVector2D TileSize;
    TileSize.X = TileWidth;
    TileSize.Y = TileHeight;

    // Local Pos
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos = CalculateEndLocalPos(TileMapComponent, OffsetTiles, TileSize, StartLocalPos);

    FTransform ActorTransform = TileMapComponent->GetOwner()->GetActorTransform();

    // WorldSpace Pos
    FVector StartPos = ActorTransform.TransformPosition(FVector(StartLocalPos, 0.0f));
    FVector EndPos = ActorTransform.TransformPosition(FVector(EndLocalPos, 0.0f));


    int32 PossibleOffset = OffsetTiles / TileInterval;
    TArray<int32> NecessarySpriteOffset;
    while (MinSpriteNum > NecessarySpriteOffset.Num())
    {
        int32 Offset = FMath::RandRange(0, PossibleOffset - 1);
        if (NecessarySpriteOffset.Contains(Offset) == false)
        {
            NecessarySpriteOffset.Add(Offset);
        }
    }

    for (int32 Offset : NecessarySpriteOffset)
    {
        int32 Index = FeatureInfoArr.GetRandomIndex();
        if (Index != 0)
        {
            float RandomScale = FMath::RandRange(1.0f, 1.5f);
            FVector LocalPos = CalculateLocalPos(TileMapComponent, StartPos.X + TileSize.X * Offset, StartPos, TileSize, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
            FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
            if ((bAllowOverlap || !RootNode->IsColliding(BoxForSprite)) && BoxForSprite.Max.X < EndPos.X && BoxForSprite.Min.X > StartPos.X)
            {
                if (SpriteNum >= MaxSpriteNum) return;
                SpriteNum++;
                CreateAndAttachSpriteComponent(TileMapComponent->GetOwner(), FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
                LocalPos.Y = UCCStageMapManager::GetInstance()->FieldTileY;
            }
        }
    }

    do
    {
        for (float i = StartPos.X; i <= EndPos.X; i += TileSize.X * TileInterval)
        {
            int32 Index = FeatureInfoArr.GetRandomIndex();
            if (Index != 0)
            {
                float RandomScale = FMath::RandRange(1.0f, 1.0f);
                FVector LocalPos = CalculateLocalPos(TileMapComponent, i, StartPos, TileSize, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
                FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
                if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
                {
                    if (SpriteNum >= MaxSpriteNum) return;
                    SpriteNum++;
                    CreateAndAttachSpriteComponent(TileMapComponent->GetOwner(), FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
                    LocalPos.Y = UCCStageMapManager::GetInstance()->FieldTileY;
                }
            }
        }
    } while (SpriteNum < MinSpriteNum);
}

FBox2D UCCSpritePlacer::GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos)
{
    FVector SpriteBoxExtent = PaperSprite->GetRenderBounds().BoxExtent;
    SpriteBoxExtent *= Scale;
    FVector2D LeftDown(FVector2D(MiddleButtonPos.X - SpriteBoxExtent.X, MiddleButtonPos.Z - SpriteBoxExtent.Z));
    FVector2D RightUp(FVector2D(MiddleButtonPos.X + SpriteBoxExtent.X, MiddleButtonPos.Z + SpriteBoxExtent.Z));
    FBox2D BoxForSprite(LeftDown, RightUp);
    return BoxForSprite;
}

float UCCSpritePlacer::CalculateYPos(bool bIsBeforePlayer)
{
    float YPos;
    if (bIsBeforePlayer)
    {
        YPos = UCCStageMapManager::GetInstance()->BeforePlayerSpriteY;
    }
    else
    {
        YPos = UCCStageMapManager::GetInstance()->AfterPlayerSpriteY;
    }
    return YPos;
}

void UCCSpritePlacer::InitializeSprite(UPaperTileMapComponent* TileMapComponent, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight)
{
    TileMapHeight = Rows;
    TileMapWidth = Columns;
    TileWidth = NewTileWidth;
    TileHeight = NewTileHeight;

    RootNode = NewObject<UCCBoxQuadTreeNode>();
    RootNode->Initialize(FVector2D(0, -TileMapHeight * 512), FVector2D(TileMapWidth * 512, 0), 10);

    for (UCCPlatform* Platform : UCCStageMapManager::GetStageMap()->GetPlatformArr())
    {
        for (UCCField* Field : Platform->GetFieldArr())
        {
            CreateSpriteByType(TileMapComponent, Field->GetFieldType(), Field->GetStartPos().X, Field->GetStartPos().Y, Field->GetLength());
        }
    }

    for (int32 i = 0; i < TileMapWidth; i++)
    {
        for (int32 j = 0; j < TileMapHeight; j++)
        {
            int32 k = TileMapComponent->GetTile(i, j, 0).GetTileIndex();
            if (TileMapComponent->GetTile(i, j, 0).GetTileIndex() == 1)
            {
                CreatBasicSprite(TileMapComponent, i, j, 1);
            }
        }
    }

}

FVector2D UCCSpritePlacer::GetTileSize(UPaperTileMapComponent* TileMapComponent) const
{
    return FVector2D(TileMapComponent->TileMap->TileWidth, TileMapComponent->TileMap->TileHeight);
}

/// <summary>
/// Ensure end local pos in in bound of TileMap
/// </summary>
/// <param name="TileMapComponent">
/// TileMap to find end local pos of
/// </param>
/// <param name="OffsetTiles">
/// tile count from start tile
/// </param>
/// <param name="TileSize">
/// tile size in WorldSpace
/// </param>
/// <param name="StartLocalPos">
/// local pos of start tile
/// </param>
/// <returns></returns>
FVector2D UCCSpritePlacer::CalculateEndLocalPos(UPaperTileMapComponent* TileMapComponent, int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos) const
{
    if (OffsetTiles * TileSize.X < TileMapComponent->TileMap->TileWidth *  TileMapWidth - StartLocalPos.X)
        return StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
    else
        return StartLocalPos + FVector2D(TileMapComponent->TileMap->TileWidth * ( TileMapWidth - 1) - StartLocalPos.X, 0);
}

void UCCSpritePlacer::CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer)
{
    if (!Owner || !FeatureSprite)
    {
        return;
    }

    UPaperSpriteComponent* NewSpriteComponent = NewObject<UPaperSpriteComponent>(Owner);
    if (NewSpriteComponent)
    {
        NewSpriteComponent->SetupAttachment(Owner->GetRootComponent());
        NewSpriteComponent->RegisterComponent();
        NewSpriteComponent->SetSprite(FeatureSprite);
        NewSpriteComponent->SetRelativeLocation(LocalPos);
        NewSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
        NewSpriteComponent->SetRelativeScale3D(FVector(RandomScale, 1, RandomScale));
        if (bIsBeforePlayer)
        {
            NewSpriteComponent->SetTranslucentSortPriority(BeforPlayerOrder);
            BeforPlayerOrder++;
        }
        else
        {
            NewSpriteComponent->SetTranslucentSortPriority(AfterPlayerOrder);
            AfterPlayerOrder++;
        }
        

        if (bAddToCollisionTree)
        {
            RootNode->Insert(BoxForSprite);
        }
        FeatureSpriteComponentArr.Add(NewSpriteComponent);
    }
}

FVector UCCSpritePlacer::CalculateLocalPos(UPaperTileMapComponent* TileMapComponent, float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite, bool bIsBeforePlayer)
{
    return FVector(XPos, CalculateYPos(bIsBeforePlayer), TileMapComponent->GetRelativeLocation().Y + TileSize.Y / 2 - StartPos.Y);
}

void UCCSpritePlacer::CreateSpriteByType(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 Length)
{
    switch (CurrentType)
    {
    case EFieldType::WatersideField:
        CreatWaterSideSprite(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::AsphaltField:
        CreateAsphaltSprite(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::CaveField:
        CreateCaveSprite(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::HillField:
        CreatHillSprite(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::RaccoonHouseField:
        CreateRaccoonHouseSprite(TileMapComponent, Column, Row, Length);
        break;
    case EFieldType::DogHouseField:
        CreateDogHouseSprite(TileMapComponent, Column, Row, Length);
        break;
    default:
        break;
    }
}

void UCCSpritePlacer::CreatBasicSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5f, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::GrassFeature), false, true);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5f, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::PlantFeature), false, true);
}

void UCCSpritePlacer::CreatWaterSideSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::WeedFeature), true, true);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::StoneFeature), true, true);
}

void UCCSpritePlacer::CreateAsphaltSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::BuildingFeature), true, false);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::CarFeature), true, true);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::LightFeature), true, true);
}

void UCCSpritePlacer::CreateCaveSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RatCaveFeature), true, true, false, 4, 4);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}

void UCCSpritePlacer::CreatHillSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    ;
}

void UCCSpritePlacer::CreateRaccoonHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RacconHouseFeature), true, true, false, 1, 1);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}

void UCCSpritePlacer::CreateDogHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::DogHouseFeature), true, true, false, 1, 1);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, *UCCStageMapManager::GetInstance()->FeatureInfoMap.Find(EFeatureType::RockFeature), false, true);
}