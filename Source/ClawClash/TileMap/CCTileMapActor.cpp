// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"

#include "PaperTileMapComponent.h"
#include "PaperSprite.h"
#include "GameFramework/Character.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/CCGameInstance.h"
#include "ClawClash/Managers/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"

#include "CCFloor.h"
#include "CCPlatform.h"
#include "CCField.h"
#include "ClawClash/TileMap/CCStageMap.h"

#include "ClawClash/TileMap/CCSpritePlacer.h"
#include "ClawClash/TileMap/CCTilePlacer.h"

ACCTileMapActor::ACCTileMapActor()
{
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    BackgroundComponent->SetupAttachment(RootComponent);
    TilePlacer = CreateDefaultSubobject<UCCTilePlacer>(TEXT("TileMapPlacer"));
    SpritePlacer = CreateDefaultSubobject<UCCSpritePlacer>(TEXT("SpritePlacer"));
}

void ACCTileMapActor::InitializeTileMap()
{
    if (TilePlacer && FieldTileMapComponent && FieldTileSet)
    {
        int32 TileMapWidth = UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth;
        int32 TileMapHeight = UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight;
        float TileWidth = UCCManagers::GetInstance()->GetStageMapManager()->TileWidth;
        float TileHeight = UCCManagers::GetInstance()->GetStageMapManager()->TileHeight;

        TilePlacer->InitializeTileSet(FieldTileSet);
        TilePlacer->InitializeTileMap(FieldTileMapComponent, FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
        TilePlacer->CreatAllField(FieldTileMapComponent);
        TilePlacer->FillInEmpty(FieldTileMapComponent);
        TilePlacer->FillInBasic(FieldTileMapComponent);
    }
}


void ACCTileMapActor::InitializeBackground()
{
    if (BackGroundSprite && BackgroundComponent)
    {
        float TileMapWidthInPixels = UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth * 512;
        float TileMapHeightInPixels = UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight * 512;

        BackgroundComponent->SetSprite(BackGroundSprite);
        FVector2D OriginalSize = BackgroundComponent->GetSprite()->GetSourceSize();
        BackgroundComponent->SetRelativeLocation(FVector(Player->GetActorLocation().X, BackgroundY - FieldTileY, Player->GetActorLocation().Z));
        BackgroundComponent->SetRelativeScale3D(FVector(TileMapWidthInPixels / (2.0 * OriginalSize.X), 1.0f, TileMapWidthInPixels / (2.0 * OriginalSize.X)));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
}

void ACCTileMapActor::PlaceFieldSprites()
{
    int32 TileMapWidth = UCCManagers::GetInstance()->GetStageMapManager()->TileMapWidth;
    int32 TileMapHeight = UCCManagers::GetInstance()->GetStageMapManager()->TileMapHeight;
    float TileWidth = UCCManagers::GetInstance()->GetStageMapManager()->TileWidth;
    float TileHeight = UCCManagers::GetInstance()->GetStageMapManager()->TileHeight;

    SpritePlacer->InitializeSprite(FieldTileMapComponent, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
}

void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    BackgroundY = UCCManagers::GetInstance()->GetStageMapManager()->BackgroundY;
    PlayerY = UCCManagers::GetInstance()->GetStageMapManager()->PlayerY;
    FieldTileY = UCCManagers::GetInstance()->GetStageMapManager()->FieldTileY;

    // Initialize Player
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    UCCManagers::GetInstance()->GetStageMapManager()->CreateStageMap();
    InitializeTileMap();
    FieldTileMapComponent->RebuildCollision();
    PlaceFieldSprites();
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