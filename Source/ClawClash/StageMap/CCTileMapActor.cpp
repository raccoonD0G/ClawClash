// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"

#include "PaperTileMapComponent.h"
#include "PaperSprite.h"
#include "GameFramework/Character.h"
#include "PaperTileMap.h"
#include "PaperSpriteComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/GameInstance/CCGameInstance.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCGameManager.h"

#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"
#include "ClawClash/StageMap/StageMapParts/CCField.h"
#include "ClawClash/StageMap/CCStageMapDef.h"

#include "ClawClash/StageMap/StageMapPlacer/CCSpritePlacer.h"
#include "ClawClash/StageMap/StageMapPlacer/CCTilePlacer.h"

ACCTileMapActor::ACCTileMapActor()
{
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    BackgroundComponent->SetupAttachment(RootComponent);
    TilePlacer = CreateDefaultSubobject<UCCTilePlacer>(TEXT("TileMapPlacer"));
    SpritePlacer = CreateDefaultSubobject<UCCSpritePlacer>(TEXT("SpritePlacer"));

    FieldTileMapComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACCTileMapActor::InitializeTileMap()
{
    if (TilePlacer && FieldTileMapComponent && FieldTileSet)
    {
        int32 TileMapWidth = UCCStageMapManager::GetInstance()->TileMapWidth;
        int32 TileMapHeight = UCCStageMapManager::GetInstance()->TileMapHeight;
        float TileWidth = UCCStageMapManager::GetInstance()->TileWidth;
        float TileHeight = UCCStageMapManager::GetInstance()->TileHeight;

        TilePlacer->InitializeTileSet(FieldTileSet);
        TilePlacer->InitializeTileMap(FieldTileMapComponent, FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
        TilePlacer->CreatAllField(FieldTileMapComponent);
        TilePlacer->FillInBasic(FieldTileMapComponent);

    }
}


void ACCTileMapActor::InitializeBackground()
{
    int32 Index = FMath::RandRange(0, BackGroundSpriteArr.Num() - 1);
    BackGroundSprite = BackGroundSpriteArr[Index];

    if (BackGroundSprite && BackgroundComponent)
    {
        float TileMapWidthInPixels = UCCStageMapManager::GetInstance()->TileMapWidth * 512;
        float TileMapHeightInPixels = UCCStageMapManager::GetInstance()->TileMapHeight * 512;

        BackgroundComponent->SetSprite(BackGroundSprite);
        BackgroundComponent->SetRelativeLocation(FVector(Player->GetActorLocation().X, BackgroundY - FieldTileY, Player->GetActorLocation().Z));
        BackgroundComponent->SetRelativeScale3D(FVector(TileMapWidthInPixels / (2.0 * BackgroundComponent->GetSprite()->GetRenderBounds().BoxExtent.X * 2), 1.0f, TileMapWidthInPixels / (2.0 * BackgroundComponent->GetSprite()->GetRenderBounds().BoxExtent.X * 2)));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
}

void ACCTileMapActor::PlaceFieldSprites()
{
    int32 TileMapWidth = UCCStageMapManager::GetInstance()->TileMapWidth;
    int32 TileMapHeight = UCCStageMapManager::GetInstance()->TileMapHeight;
    float TileWidth = UCCStageMapManager::GetInstance()->TileWidth;
    float TileHeight = UCCStageMapManager::GetInstance()->TileHeight;

    SpritePlacer->InitializeSprite(FieldTileMapComponent, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
}

void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    BackgroundY = UCCStageMapManager::GetInstance()->BackgroundY;
    PlayerY = UCCStageMapManager::GetInstance()->PlayerY;
    FieldTileY = UCCStageMapManager::GetInstance()->FieldTileY;

    // Initialize Player
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    InitializeTileMap();
    FieldTileMapComponent->RebuildCollision();
    PlaceFieldSprites();
    InitializeBackground();
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