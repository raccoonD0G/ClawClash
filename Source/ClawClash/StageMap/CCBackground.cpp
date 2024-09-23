// Fill out your copyright notice in the Description page of Project Settings.


#include "CCBackground.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Character.h"
#include "PaperSprite.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include <ClawClash/Managers/LayerManager/CCLayerManager.h>

// Sets default values
ACCBackground::ACCBackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    UpSizeAmount = 0.5f;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    RootComponent = BackgroundComponent;
}

// Called when the game starts or when spawned
void ACCBackground::BeginPlay()
{
	Super::BeginPlay();

    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    int32 Index = FMath::RandRange(0, BackGroundSpriteArr.Num() - 1);
    BackGroundSprite = BackGroundSpriteArr[Index];

    if (BackGroundSprite && BackgroundComponent)
    {
        float TileMapWidthInPixels = StageMap->GetTileWidth() * StageMap->GetTileMapWidth();
        float TileMapHeightInPixels = StageMap->GetTileHeight() * StageMap->GetTileMapHeight();

        BackgroundComponent->SetSprite(BackGroundSprite);

        float SpriteWidth = BackGroundSprite->GetBakedTexture()->GetSizeX();
        float SpriteHeight = BackGroundSprite->GetBakedTexture()->GetSizeY();

        BackgroundComponent->SetWorldScale3D(FVector(TileMapWidthInPixels / SpriteWidth * UpSizeAmount, 1.0f, TileMapHeightInPixels / SpriteHeight * UpSizeAmount));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
}

// Called every frame
void ACCBackground::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    float TileMapWidthInPixels = StageMap->GetTileWidth() * StageMap->GetTileMapWidth();
    float TileMapHeightInPixels = StageMap->GetTileHeight() * StageMap->GetTileMapHeight();

    // Get Player Pos
    FVector PlayerPos = Player->GetActorLocation();
    PlayerPos = PlayerPos - StageMap->GetActorLocation() + TileMapHeightInPixels;

    float PlayerStageMapXPos = PlayerPos.X / TileMapWidthInPixels;
    float PlayerStageMapZPos = PlayerPos.Z / TileMapHeightInPixels;

    // Set Background Pos
    float XPos = (0.5f - PlayerStageMapXPos) * TileMapWidthInPixels * UpSizeAmount;
    XPos += TileMapWidthInPixels / 4;
    float ZPos = (0.5f - PlayerStageMapZPos) * TileMapHeightInPixels * UpSizeAmount;
    BackgroundComponent->SetRelativeLocation(FVector(XPos + Player->GetActorLocation().X, UCCLayerManager::GetBackgroundY(), ZPos + Player->GetActorLocation().Z));
}


