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

void UCCTileMapLineParts::PlaceSprites(float TileInterval, const FCCFeatureInfoArrContainer& FeatureInfoArr, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    int32 OffsetTiles = Length - 1;

    int32 SpriteNum = 0;

    FVector2D TileSize;
    TileSize.X = OwningTileMap->GetTileWidth();
    TileSize.Y = OwningTileMap->GetTileHeight();

    // Local Pos
    FVector2D StartLocalPos;
    StartLocalPos.X = TileMapPos.X * TileSize.X;
    StartLocalPos.Y = TileMapPos.Y * TileSize.Y;
    FVector2D EndLocalPos = CalculateEndLocalPos(OffsetTiles, TileSize, StartLocalPos);

    FTransform ActorTransform = OwningTileMap->GetActorTransform();

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
            FVector LocalPos = CalculateLocalPos(StartPos.X + TileSize.X * Offset, StartPos, TileSize, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
            FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
            if ((bAllowOverlap || !OwningTileMap->GetRootNode()->IsColliding(BoxForSprite)) && BoxForSprite.Max.X < EndPos.X && BoxForSprite.Min.X > StartPos.X)
            {
                if (SpriteNum >= MaxSpriteNum) return;
                SpriteNum++;
                CreateAndAttachSpriteComponent(OwningTileMap, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
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
                FVector LocalPos = CalculateLocalPos(i, StartPos, TileSize, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, bIsBeforePlayer);
                FBox2D BoxForSprite = GetSpriteBox(FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, RandomScale, LocalPos);
                if (bAllowOverlap || !OwningTileMap->GetRootNode()->IsColliding(BoxForSprite))
                {
                    if (SpriteNum >= MaxSpriteNum) return;
                    SpriteNum++;
                    CreateAndAttachSpriteComponent(OwningTileMap, FeatureInfoArr.FeatureInfoArr[Index].FeatureSprite, LocalPos, bAddToCollisionTree, BoxForSprite, RandomScale, bIsBeforePlayer);
                    LocalPos.Y = UCCStageMapManager::GetInstance()->FieldTileY;
                }
            }
        }
    } while (SpriteNum < MinSpriteNum);
}

FBox2D UCCTileMapLineParts::GetSpriteBox(UPaperSprite* PaperSprite, float Scale, FVector MiddleButtonPos)
{
    FVector SpriteBoxExtent = PaperSprite->GetRenderBounds().BoxExtent;
    SpriteBoxExtent *= Scale;
    FVector2D LeftDown(FVector2D(MiddleButtonPos.X - SpriteBoxExtent.X, MiddleButtonPos.Z - SpriteBoxExtent.Z));
    FVector2D RightUp(FVector2D(MiddleButtonPos.X + SpriteBoxExtent.X, MiddleButtonPos.Z + SpriteBoxExtent.Z));
    FBox2D BoxForSprite(LeftDown, RightUp);
    return BoxForSprite;
}

float UCCTileMapLineParts::CalculateYPos(bool bIsBeforePlayer)
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


FVector2D UCCTileMapLineParts::GetTileSize() const
{
    return FVector2D(OwningTileMap->GetTileWidth(), OwningTileMap->GetTileHeight());
}

/// <summary>
/// Ensure end local pos in in bound of TileMap
/// </summary>
/// </param>
/// <param name="TileSize">
/// tile size in WorldSpace
/// </param>
/// <param name="StartLocalPos">
/// local pos of start tile
/// </param>
/// <returns></returns>
FVector2D UCCTileMapLineParts::CalculateEndLocalPos(int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos) const
{
    if (OffsetTiles * TileSize.X < OwningTileMap->GetTileWidth() * OwningTileMap->GetTileMapWidth() - StartLocalPos.X)
        return StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
    else
        return StartLocalPos + FVector2D(OwningTileMap->GetTileWidth() * (OwningTileMap->GetTileMapWidth() - 1) - StartLocalPos.X, 0);
}

void UCCTileMapLineParts::CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree, FBox2D BoxForSprite, float RandomScale, bool bIsBeforePlayer)
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
            NewSpriteComponent->SetTranslucentSortPriority(OwningTileMap->GetBeforePlayerOrder());
            OwningTileMap->AddBeforPlayerOrder();
        }
        else
        {
            NewSpriteComponent->SetTranslucentSortPriority(OwningTileMap->GetAfterPlayerOrder());
            OwningTileMap->AddAfterPlayerOrder();
        }


        if (bAddToCollisionTree)
        {
            OwningTileMap->GetRootNode()->Insert(BoxForSprite);
        }
        OwningTileMap->AddSpriteComponentArr(NewSpriteComponent);
    }
}

FVector UCCTileMapLineParts::CalculateLocalPos(float XPos, FVector StartPos, FVector2D TileSize, UPaperSprite* FeatureSprite, bool bIsBeforePlayer)
{
    return FVector(XPos, CalculateYPos(bIsBeforePlayer), OwningTileMap->GetComponentByClass<UPaperTileMapComponent>()->GetRelativeLocation().Y + TileSize.Y / 2 - StartPos.Y);
}