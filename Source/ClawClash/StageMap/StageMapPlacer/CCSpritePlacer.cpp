// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpritePlacer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"
#include "ClawClash/StageMap/CCBoxQuadTreeNode.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/StageMapParts/CCField.h"
#include "ClawClash/StageMap/CCStageMap.h"
#include "PaperTileLayer.h"

TArray<FVector> UCCSpritePlacer::PlaceSprites(UPaperTileMapComponent* TileMapComponent, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<FCCFeatureInfo>& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    OffsetTiles--;

    int32 SpriteNum = 0;
    TArray<FVector> PlacedSpritePositions;

    if (!TileMapComponent->TileMap)
    {
        return PlacedSpritePositions;
    }

    FVector2D TileSize;
    TileSize.X = TileWidth;
    TileSize.Y = TileHeight;
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos = CalculateEndLocalPos(TileMapComponent, OffsetTiles, TileSize, StartLocalPos);

    FTransform ActorTransform = TileMapComponent->GetOwner()->GetActorTransform();
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
        int32 Index = 0; 
        while (Index != 0)
        {
            Index = UCCManagers::GetInstance()->GetRandomIndexByProbability(FeatureInfoArr);
        }

        if (Index != 0)
        {
            Index--;
            float RandomScale = FMath::RandRange(0.8f, 1.0f);
            FVector LocalPos = CalculateLocalPos(TileMapComponent, StartPos.X + TileSize.X * Offset, StartPos, TileSize, FeatureInfoArr[Index].FeatureSprite, RandomScale, bIsBeforePlayer);
            FBox2D BoxForSprite(FVector2D(LocalPos.X, LocalPos.Z), FVector2D(LocalPos.X + FeatureInfoArr[Index].FeatureSprite->GetRenderBounds().BoxExtent.X * 3, LocalPos.Z + FeatureInfoArr[Index].FeatureSprite->GetRenderBounds().BoxExtent.Z * 3));
            if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
            {
                if (SpriteNum >= MaxSpriteNum) return PlacedSpritePositions;
                SpriteNum++;
                CreateAndAttachSpriteComponent(TileMapComponent->GetOwner(), FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale);
                LocalPos.Y = FieldTileY;
                PlacedSpritePositions.Add(LocalPos);
            }
        }
    }


    do
    {
        for (float i = StartPos.X + TileSize.X * TileInterval; i <= EndPos.X; i += TileSize.X * TileInterval)
        {
            int32 Index = UCCManagers::GetInstance()->GetRandomIndexByProbability(FeatureInfoArr);
            if (Index != 0)
            {
                Index--;
                float RandomScale = FMath::RandRange(1.0f, 1.5f);
                FVector LocalPos = CalculateLocalPos(TileMapComponent, i, StartPos, TileSize, FeatureInfoArr[Index].FeatureSprite, RandomScale, bIsBeforePlayer);
                FBox2D BoxForSprite(FVector2D(LocalPos.X, LocalPos.Z), FVector2D(LocalPos.X + FeatureInfoArr[Index].FeatureSprite->GetRenderBounds().BoxExtent.X * 3, LocalPos.Z + FeatureInfoArr[Index].FeatureSprite->GetRenderBounds().BoxExtent.Z * 3));

                if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
                {
                    if (SpriteNum >= MaxSpriteNum) return PlacedSpritePositions;
                    SpriteNum++;
                    CreateAndAttachSpriteComponent(TileMapComponent->GetOwner(), FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale);
                    LocalPos.Y = FieldTileY;
                    PlacedSpritePositions.Add(LocalPos);
                }
            }
        }
    } while (SpriteNum < MinSpriteNum);

    return PlacedSpritePositions;
}

float UCCSpritePlacer::CalculateYPos(bool bIsBeforePlayer)
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

void UCCSpritePlacer::InitializeSprite(UPaperTileMapComponent* TileMapComponent, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight)
{
    TileMapHeight = Rows;
    TileMapWidth = Columns;
    TileWidth = NewTileWidth;
    TileHeight = NewTileHeight;
    BackgroundY = UCCManagers::GetInstance()->GetStageMapManager()->BackgroundY;
    PlayerY = UCCManagers::GetInstance()->GetStageMapManager()->PlayerY;
    FieldTileY = UCCManagers::GetInstance()->GetStageMapManager()->FieldTileY;
    CurrentBeforePlayerFeatureY = 0.1;
    CurrentAfterPlayerFeatureY = 0.1;

    RootNode = NewObject<UCCBoxQuadTreeNode>();
    RootNode->Initialize(FVector2D(0, -UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight * 512), FVector2D(UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth * 512, 0), 10);

    for (UCCPlatform* Platform : UCCManagers::GetInstance()->GetStageMapManager()->GetStageMap().GetPlatformArr())
    {
        TreePosArr.Append(PlaceSprites(TileMapComponent, FVector2D(Platform->GetStartPos().X, Platform->GetStartPos().Y), Platform->GetLength(), FMath::RandRange(30.0, 60.0), UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::TreeFeature)->FeatureInfoArr, false, true, false));
        for (UCCField* Field : Platform->GetFieldArr())
        {
            CreateSpriteByType(TileMapComponent, Field->GetFieldType(), Field->GetStartPos().X, Field->GetStartPos().Y, Field->GetLength());
        }
    }
}

FVector2D UCCSpritePlacer::GetTileSize(UPaperTileMapComponent* TileMapComponent) const
{
    return FVector2D(TileMapComponent->TileMap->TileWidth, TileMapComponent->TileMap->TileHeight);
}

FVector2D UCCSpritePlacer::CalculateEndLocalPos(UPaperTileMapComponent* TileMapComponent, int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos) const
{
    if (OffsetTiles * TileSize.X < TileMapComponent->TileMap->TileWidth * UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth - StartLocalPos.X)
        return StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
    else
        return StartLocalPos + FVector2D(TileMapComponent->TileMap->TileWidth * (UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth - 1) - StartLocalPos.X, 0);
}

void UCCSpritePlacer::CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale)
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

        if (bAddToCollisionTree)
        {
            RootNode->Insert(BoxForSprite);
        }
        FeatureSpriteComponentArr.Add(NewSpriteComponent);
    }
}

FVector UCCSpritePlacer::CalculateLocalPos(UPaperTileMapComponent* TileMapComponent, float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite, float RandomScale, bool bIsBeforePlayer)
{
    return FVector(XPos, CalculateYPos(bIsBeforePlayer), TileMapComponent->GetRelativeLocation().Y + TileSize.Y / 2 - StartPos.Y);
}

TArray<FVector> UCCSpritePlacer::CreateSpriteByType(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 Length)
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
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreatWaterSideSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::WeedFeature)->FeatureInfoArr, true, true);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::StoneFeature)->FeatureInfoArr, true, true);
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreateAsphaltSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::BuildingFeature)->FeatureInfoArr, true, false);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::CarFeature)->FeatureInfoArr, true, true);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::LightFeature)->FeatureInfoArr, true, true);
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreateCaveSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RatCaveFeature)->FeatureInfoArr, true, true, false, 4, 4);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreatHillSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreateRaccoonHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RacconHouseFeature)->FeatureInfoArr, true, true, false, 1, 1);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    return TArray<FVector>();
}

TArray<FVector> UCCSpritePlacer::CreateDogHouseSprite(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length)
{
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 1.0, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::DogHouseFeature)->FeatureInfoArr, true, true, false, 1, 1);
    PlaceSprites(TileMapComponent, FVector2D(Column, Row), Length, 0.5, UCCManagers::GetInstance()->GetStageMapManager()->FeatureInfoMap.Find(EFeatureType::RockFeature)->FeatureInfoArr, false, true);
    return TArray<FVector>();
}