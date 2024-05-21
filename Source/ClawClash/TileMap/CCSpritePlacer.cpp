// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpritePlacer.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"
#include "ClawClash/TileMap/CCBoxQuadTreeNode.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

TArray<FVector> UCCSpritePlacer::PlaceSprites(UPaperTileMapComponent* TileMapComponent, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, const TArray<UPaperSprite*>& SpriteArray, bool bIsBeforePlayer, bool bAllowOverlap, bool bAddToCollisionTree, int32 MinSpriteNum, int32 MaxSpriteNum)
{
    TArray<FVector> PlacedSpritePositions;

    if (!TileMapComponent || SpriteArray.Num() == 0)
    {
        return PlacedSpritePositions;
    }

    FVector2D TileSize = GetTileSize(TileMapComponent);
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos = CalculateEndLocalPos(OffsetTiles, TileSize, StartLocalPos, TileMapComponent->TileMap->MapWidth);

    for (float X = StartLocalPos.X; X <= EndLocalPos.X; X += TileSize.X * TileInterval)
    {
        int32 Index = FMath::RandRange(0, SpriteArray.Num() - 1);
        UPaperSprite* SelectedSprite = SpriteArray[Index];
        FVector LocalPos(X, StartLocalPos.Y, 0.0f); // Y와 Z 위치는 적절히 설정

        CreateAndAttachSpriteComponent(TileMapComponent->GetOwner(), SelectedSprite, LocalPos, bAddToCollisionTree);

        PlacedSpritePositions.Add(LocalPos);
    }

    return PlacedSpritePositions;
}

FVector2D UCCSpritePlacer::GetTileSize(UPaperTileMapComponent* TileMapComponent) const
{
    return FVector2D(TileMapComponent->TileMap->TileWidth, TileMapComponent->TileMap->TileHeight);
}

FVector2D UCCSpritePlacer::CalculateEndLocalPos(int32 OffsetTiles, FVector2D TileSize, FVector2D StartLocalPos, int32 MapWidth) const
{
    return StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
}

void UCCSpritePlacer::CreateAndAttachSpriteComponent(AActor* Owner, UPaperSprite* FeatureSprite, FVector LocalPos, bool bAddToCollisionTree)
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

        if (bAddToCollisionTree)
        {
            FBox2D BoxForSprite = GetSpriteComponentBounds(NewSpriteComponent);
            RootNode->Insert(BoxForSprite);
        }
        FeatureSpriteComponentArr.Add(NewSpriteComponent);
    }
}

FBox2D UCCSpritePlacer::GetSpriteComponentBounds(UPaperSpriteComponent* SpriteComponent)
{
    if (!SpriteComponent || !SpriteComponent->GetSprite())
    {
        return FBox2D();
    }

    FVector2D SpriteSize = SpriteComponent->GetSprite()->GetSourceSize();

    FVector WorldLocation = SpriteComponent->GetComponentLocation();

    FTransform ComponentTransform = SpriteComponent->GetComponentTransform();

    FVector2D BottomLeft = FVector2D(WorldLocation.X, WorldLocation.Z);
    FVector2D TopRight = FVector2D(WorldLocation.X + SpriteSize.X * ComponentTransform.GetScale3D().X,
        WorldLocation.Z + SpriteSize.Y * ComponentTransform.GetScale3D().Y);

    FBox2D SpriteBounds(BottomLeft, TopRight);

    return SpriteBounds;
}
