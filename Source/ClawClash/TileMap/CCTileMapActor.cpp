// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "CCTile.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "CCBoxQuadTreeNode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Math/UnrealMathUtility.h"


// Player Y : 50
// Field Y : 100
// Feature Y : 0
// Background Y : -100

ACCTileMapActor::ACCTileMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    BackgroundComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize Player
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // Initialize TileMap
    FieldTileMapComponent->CreateNewTileMap(TileMapWidth, TileMapHeight, 512, 512, 1.0f, true);
    FieldTileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
    FieldTileMapComponent->SetRelativeLocation(FVector(0, 100, 0));

    RootNode = NewObject<UCCBoxQuadTreeNode>(this);
    RootNode->Initialize(FVector2D(0, -TileMapHeight * 512), FVector2D(TileMapWidth * 512, 0), 10);

    // Initialize Background
    if (BackGroundSprite != NULL)
    {
        float TileMapWidthInPixels = TileMapWidth * 512;
        float TileMapHeightInPixels = TileMapHeight * 512;

        BackgroundComponent->SetSprite(BackGroundSprite);
        FVector2D OriginalSize = BackgroundComponent->GetSprite()->GetSourceSize();
        BackgroundComponent->SetRelativeLocation(FVector(Player->GetActorLocation().X, -200, Player->GetActorLocation().Z));
        BackgroundComponent->SetRelativeScale3D(FVector(TileMapWidthInPixels / (2.0 * OriginalSize.X), 1.0f, TileMapWidthInPixels / (2.0 * OriginalSize.X)));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }

    // Initialize FieldRatio
    int32 FieldTypeEnumLength = GetEnumLength(StaticEnum<EFieldType>());
    FieldRatio.SetNum(FieldTypeEnumLength);
    FieldRatio[(int32)EFieldType::BasicField] = 0.4f;
    FieldRatio[(int32)EFieldType::WatersideField] = 0.1f;
    FieldRatio[(int32)EFieldType::AsphaltField] = 0.1f;
    FieldRatio[(int32)EFieldType::CaveField] = 0.1f;
    FieldRatio[(int32)EFieldType::HillField] = 0.03f;
    FieldRatio[(int32)EFieldType::EmptyField] = 0.0f;

    // Initialize FieldInfo
    FieldInfoArr.SetNum(FieldTypeEnumLength);
    FieldInfoArr[(int32)EFieldType::HillField].MaxLength = 12;
    FieldInfoArr[(int32)EFieldType::HillField].MinLenght = 7;
    FieldInfoArr[(int32)EFieldType::WatersideField].MaxLength = 15;
    FieldInfoArr[(int32)EFieldType::WatersideField].MinLenght = 10;
    FieldInfoArr[(int32)EFieldType::CaveField].MaxLength = 15;
    FieldInfoArr[(int32)EFieldType::CaveField].MinLenght = 10;
    FieldInfoArr[(int32)EFieldType::AsphaltField].MaxLength = 15;
    FieldInfoArr[(int32)EFieldType::AsphaltField].MinLenght = 10;

    // Initialize Feature
    WeedFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EWeedFeature>()));
    WeedFeatureInfo.FeatureRatio[(int32)EWeedFeature::NoneFeature] = 2.3f;
    WeedFeatureInfo.FeatureRatio[(int32)EWeedFeature::Weed0Feature] = 0.3f;
    WeedFeatureInfo.FeatureRatio[(int32)EWeedFeature::Weed1Feature] = 0.3f;
    WeedFeatureInfo.FeatureRatio[(int32)EWeedFeature::Weed2Feature] = 0.3f;
    WeedFeatureInfo.FeatureRatio[(int32)EWeedFeature::Weed3Feature] = 0.3f;
    WeedFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Weed");

    RockFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<ERockFeature>()));
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::NoneFeature] = 2.3f;
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::Rock0Feature] = 0.3f;
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::Rock1Feature] = 0.3f;
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::Rock2Feature] = 0.3f;
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::Rock3Feature] = 0.3f;
    RockFeatureInfo.FeatureRatio[(int32)ERockFeature::Rock4Feature] = 0.3f;
    RockFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Rock");

    BuildingFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EBuildingFeature>()));
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::NoneFeature] = 1.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House0Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House1Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House2Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House3Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House4Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House5Feature] = 0.3f;
    BuildingFeatureInfo.FeatureRatio[(int32)EBuildingFeature::House6Feature] = 0.3f;
    BuildingFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Building");

    CarFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<ECarFeature>()));
    CarFeatureInfo.FeatureRatio[(int32)ECarFeature::NoneFeature] = 2.3f;
    CarFeatureInfo.FeatureRatio[(int32)ECarFeature::Car0Feature] = 0.3f;
    CarFeatureInfo.FeatureRatio[(int32)ECarFeature::Car1Feature] = 0.3f;
    CarFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Car");

    LightFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<ELightFeature>()));
    LightFeatureInfo.FeatureRatio[(int32)ELightFeature::NoneFeature] = 2.3f;
    LightFeatureInfo.FeatureRatio[(int32)ELightFeature::ShortLightFeature] = 0.3f;
    LightFeatureInfo.FeatureRatio[(int32)ELightFeature::MiddleLightFeature] = 0.3f;
    LightFeatureInfo.FeatureRatio[(int32)ELightFeature::LongLightFeature] = 0.3f;
    LightFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Light");

    StoneFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EStoneFeature>()));
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::NoneFeature] = 3.3f;
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::Stone0Feature] = 0.3f;
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::Stone1Feature] = 0.3f;
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::Stone2Feature] = 0.3f;
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::Stone3Feature] = 0.3f;
    StoneFeatureInfo.FeatureRatio[(int32)EStoneFeature::Stone4Feature] = 0.3f;
    StoneFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Stone");

    PlantFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EPlantFeature>()));
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::NoneFeature] = 1.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant0Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant1Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant2Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant3Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant4Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant5Feature] = 0.3f;
    PlantFeatureInfo.FeatureRatio[(int32)EPlantFeature::Plant6Feature] = 0.3f;
    PlantFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Plant");

    GrassFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EGrassFeature>()));
    GrassFeatureInfo.FeatureRatio[(int32)EGrassFeature::NoneFeature] = 0.0f;
    GrassFeatureInfo.FeatureRatio[(int32)EGrassFeature::Grass0Feature] = 0.3f;
    GrassFeatureInfo.FeatureRatio[(int32)EGrassFeature::Grass1Feature] = 0.3f;
    GrassFeatureInfo.FeatureRatio[(int32)EGrassFeature::Grass2Feature] = 0.3f;
    GrassFeatureInfo.FeatureRatio[(int32)EGrassFeature::Grass3Feature] = 0.3f;
    GrassFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Grass");

    WaterFeatureInfo.FeatureRatio.SetNum(GetEnumLength(StaticEnum<EWaterFeature>()));
    WaterFeatureInfo.FeatureRatio[(int32)EWaterFeature::NoneFeature] = 0.0f;
    WaterFeatureInfo.FeatureRatio[(int32)EWaterFeature::Water0Feature] = 0.3f;
    WaterFeatureInfo.SpriteArr = GetAllSpritesFromFolder("/Game/Sprite/Map/Feature/Water");

    if (FieldTileSet)
    {

        // Initialize Tiles
        int32 TileTypeEnumLength = GetEnumLength(StaticEnum<ETileType>());
        TileInfoArr.SetNum(TileTypeEnumLength);
        
        for (int32 i = 0; i < TileInfoArr.Num(); i++)
        {
            TileInfoArr[i].TileSet = FieldTileSet;
            TileInfoArr[i].PackedTileIndex = i;
        }
        
        // Fill Empty Tile
        for (int32 Row = 0; Row < TileMapHeight; Row ++)
        {
            for (int32 Column = 0; Column < TileMapWidth; Column++)
            {
                SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Empty], false);
            }
        }

        for (int32 Row = (TileMapHeight / NumOfFloor) - 1; Row <= TileMapHeight; Row += TileMapHeight / NumOfFloor)
        {
            // Creat Necessary Hill
            if (Row != TileMapHeight - 1)
            {
                int32 DefaultHillColumn = FMath::RandRange(FieldInfoArr[(int32)EFieldType::HillField].MaxLength, TileMapWidth - FieldInfoArr[(int32)EFieldType::HillField].MaxLength);
                CreateFieldByType(EFieldType::HillField, DefaultHillColumn, Row);
            }
            
            for (int32 Column = 0; Column < TileMapWidth; Column++)
            {   
                if (FieldTileMapComponent, FieldTileMapComponent->GetTile(Column, Row, 0).GetTileIndex() == (int32)ETileType::Empty)
                {
                    int32 index = GetRandomIndexByProbability(FieldRatio);
                    if (index >= 0 && index < FieldTypeEnumLength)
                    {
                        EFieldType FieldTypeToPlace = (EFieldType)index;
                        CreateFieldByType(FieldTypeToPlace, Column, Row);
                    }
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
    BackgroundComponent->SetRelativeLocation(FVector(LastBackgroundX + ((Player->GetActorLocation().X - LastPlayerX) / 1.2), -200, LastBackgroundZ + ((Player->GetActorLocation().Z - LastPlayerZ) / 1.2)));
    LastPlayerX = Player->GetActorLocation().X;
    LastPlayerZ = Player->GetActorLocation().Z;
    LastBackgroundX = BackgroundComponent->GetRelativeLocation().X;
    LastBackgroundZ = BackgroundComponent->GetRelativeLocation().Z;

}

int32 ACCTileMapActor::GetRandomIndexByProbability(const TArray<float>& Probabilities)
{
    if (Probabilities.Num() == 0)
    {
        return -1;
    }

    float TotalProbability = 0.f;
    for (float Probability : Probabilities)
    {
        TotalProbability += Probability;
    }

    float RandomNumber = FMath::FRandRange(0.0f, TotalProbability);

    float AccumulatedProbability = 0.f;
    for (int32 i = 0; i < Probabilities.Num(); ++i)
    {
        AccumulatedProbability += Probabilities[i];
        if (RandomNumber <= AccumulatedProbability)
        {
            if (Probabilities.Num() == 5 && Probabilities[0] < 0.0001 && i == 0)
            {
                UE_LOG(LogTemp, Log, TEXT("Error"));
            }
            return i;
        }
    }

    return Probabilities.Num() - 1;
}

void ACCTileMapActor::CreateFieldByType(EFieldType CurrentType, int32 Column, int32 Row)
{
    
    switch (CurrentType)
    {
    case EFieldType::BasicField:
        CreatBasic(Column, Row);
        break;
    case EFieldType::WatersideField:
        CreatWaterSide(Column, Row);
        break;
    case EFieldType::AsphaltField:
        CreateAsphalt(Column, Row);
        break;
    case EFieldType::CaveField:
        CreateCave(Column, Row);
        break;
    case EFieldType::HillField:
        CreatHill(Column, Row, 4);
        break;
    case EFieldType::EmptyField:
        break;
    }
}

void ACCTileMapActor::CreatBasic(int32 Column, int32 Row)
{
    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)EFieldType::BasicField]);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), 1, 0.5, GrassFeatureInfo.SpriteArr, GrassFeatureInfo.FeatureRatio, 51.0, true);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), 1, 0.2, PlantFeatureInfo.SpriteArr, PlantFeatureInfo.FeatureRatio, 2.0, true);
}

void ACCTileMapActor::CreatHill(int32 Column, int32 Row, int32 StairLength)
{
    if (Row == TileMapHeight - 1)
    {
        CreatBasic(Column, Row);
        return;
    }

    int32 LengthOfHill = FMath::RandRange(FieldInfoArr[(int32)EFieldType::HillField].MinLenght, FieldInfoArr[(int32)EFieldType::HillField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfHill) == false)
    {
        CreatBasic(Column, Row);
        return;
    }

    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::HillLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfHill; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::HillSpace]);
        SetTileIfPossible(FieldTileMapComponent, i, Row + TileMapHeight / NumOfFloor / 4, 0, TileInfoArr[(int32)ETileType::Hill]);
    }
    for (int32 i = Column + 1 - StairLength; i < Column + 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (TileMapHeight / NumOfFloor) / 4 * 2, 0, TileInfoArr[(int32)ETileType::Hill]);
    }
    for (int32 i = Column + LengthOfHill; i < Column + LengthOfHill + StairLength; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (TileMapHeight / NumOfFloor) / 4 * 2, 0, TileInfoArr[(int32)ETileType::Hill]);
    }
    for (int32 i = Column + 1 - StairLength * 2; i < Column + 1 - StairLength; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (TileMapHeight / NumOfFloor) / 4 * 3, 0, TileInfoArr[(int32)ETileType::Hill]);
    }
    for (int32 i = Column + LengthOfHill + StairLength; i < Column + LengthOfHill + StairLength * 2; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row + (TileMapHeight / NumOfFloor) / 4 * 3, 0, TileInfoArr[(int32)ETileType::Hill]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfHill, Row, 0, TileInfoArr[(int32)ETileType::HillRight]);
}

void ACCTileMapActor::CreatWaterSide(int32 Column, int32 Row)
{
    int32 LengthOfField = FMath::RandRange(FieldInfoArr[(int32)EFieldType::WatersideField].MinLenght, FieldInfoArr[(int32)EFieldType::WatersideField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfField) == false)
    {
        CreatBasic(Column, Row);
        return;
    }
    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::WatersideLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfField - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Waterside]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfField - 1, Row, 0, TileInfoArr[(int32)ETileType::WatersideRight]);

    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, WaterFeatureInfo.SpriteArr, WaterFeatureInfo.FeatureRatio, 51.0, true);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, StoneFeatureInfo.SpriteArr, StoneFeatureInfo.FeatureRatio, 1.0, true);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, WeedFeatureInfo.SpriteArr, WeedFeatureInfo.FeatureRatio, 2.0, true);

}

void ACCTileMapActor::CreateCave(int32 Column, int32 Row)
{
    int32 LengthOfField = FMath::RandRange(FieldInfoArr[(int32)EFieldType::CaveField].MinLenght, FieldInfoArr[(int32)EFieldType::CaveField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfField) == false)
    {
        CreatBasic(Column, Row);
        return;
    }

    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::CaveLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfField - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Cave]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfField - 1, Row, 0, TileInfoArr[(int32)ETileType::CaveRight]);

    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5,  RockFeatureInfo.SpriteArr, RockFeatureInfo.FeatureRatio, true);
}

void ACCTileMapActor::CreateAsphalt(int32 Column, int32 Row)
{
    int32 LengthOfField = FMath::RandRange(FieldInfoArr[(int32)EFieldType::AsphaltField].MinLenght, FieldInfoArr[(int32)EFieldType::AsphaltField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfField) == false)
    {
        CreatBasic(Column, Row);
        return;
    }

    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::AsphaltLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfField - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Asphalt]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfField - 1, Row, 0, TileInfoArr[(int32)ETileType::AsphaltRight]);

    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, BuildingFeatureInfo.SpriteArr, BuildingFeatureInfo.FeatureRatio, 1.0, false, true);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, CarFeatureInfo.SpriteArr, CarFeatureInfo.FeatureRatio, 2.0, true);
    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfField, 0.5, LightFeatureInfo.SpriteArr, LightFeatureInfo.FeatureRatio, 3.0, true);
}

void ACCTileMapActor::PlaceSpritesOnTileMap(TObjectPtr<UPaperTileMap> TileMap, FVector2D StartingTile, int32 OffsetTiles, float TileInterval, TArray<TObjectPtr<UPaperSprite>> SpriteToPlace, TArray<float> RatioArr, float YPos, bool bAllowOverlap, bool bAddToCollisionTree)
{
    OffsetTiles--;

    if (!TileMap)
    {
        return;
    }

    FVector2D TileSize(TileMap->TileWidth, TileMap->TileHeight);
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos;
    if (OffsetTiles * TileSize.X < TileMap->TileWidth * TileMapWidth - StartLocalPos.X)
        EndLocalPos = StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);
    else
        EndLocalPos = StartLocalPos + FVector2D(TileMap->TileWidth * (TileMapWidth - 1) - StartLocalPos.X, 0);

    FTransform ActorTransform = GetActorTransform();
    FVector StartWorldPos = ActorTransform.TransformPosition(FVector(StartLocalPos, 0.0f));
    FVector EndWorldPos = ActorTransform.TransformPosition(FVector(EndLocalPos, 0.0f));

    for (float i = StartWorldPos.X; i <= EndWorldPos.X; i += TileSize.X * TileInterval)
    {
        int32 Index = GetRandomIndexByProbability(RatioArr);
        if (Index != 0)
        {
            Index--;
            FVector WorldPos(i, YPos - UKismetMathLibrary::RandomFloatInRange(0.0f, 0.99f) - FieldTileMapComponent->GetRelativeLocation().Y, FieldTileMapComponent->GetRelativeLocation().Y + -StartWorldPos.Y + TileSize.Y + (SpriteToPlace[Index]->GetSourceSize().Y - 512) / 2);
            FBox2D BoxForSprite(FVector2D(WorldPos.X, WorldPos.Z), FVector2D(WorldPos.X + SpriteToPlace[Index]->GetSourceSize().X, WorldPos.Z + SpriteToPlace[Index]->GetSourceSize().Y));

            if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
            {
                TObjectPtr<UPaperSpriteComponent> NewSprite = NewObject<UPaperSpriteComponent>(this);
                if (NewSprite)
                {
                    NewSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                    NewSprite->RegisterComponent();
                    NewSprite->SetVisibility(true);
                    NewSprite->SetSprite(SpriteToPlace[Index]);
                    NewSprite->SetRelativeLocation(WorldPos);
                    NewSprite->SetCollisionProfileName(TEXT("NoCollision"));
                    NewSprite->SetMaterial(0, DefaultSpriteMaterial);
                    NewSprite->bReceivesDecals = false;
                    NewSprite->CastShadow = false;

                    bool bShouldFlip = UKismetMathLibrary::RandomBool();
                    float NewScaleX = bShouldFlip ? -1.0f : 1.0f;
                    NewSprite->SetRelativeScale3D(FVector(NewScaleX, 1, 1));

                    if (bAddToCollisionTree == true) RootNode->Insert(BoxForSprite);
                    FeatureSpriteComponentArr.Add(NewSprite);
                }
            }
        }
        
    }
}




bool ACCTileMapActor::CheckAllEmpty(int32 Column, int32 Row, int32 Length)
{
    for (int32 i = Column; i < Column + Length; i++)
    {
        if (i >= 0 && i < TileMapWidth && Row >= 0 && Row < TileMapHeight)
        {
            if (FieldTileMapComponent->GetTile(i, Row, 0).GetTileIndex() == (int32)ETileType::Empty);
            else return false;
        }
        else
        {
            false;
        }
    }

    return true;
}

int32 ACCTileMapActor::GetEnumLength(TObjectPtr<UEnum> TargetEnum)
{
    return TargetEnum->NumEnums() - 1;
}

void ACCTileMapActor::SetTileIfPossible(TObjectPtr<UPaperTileMapComponent> TileMapComponent, int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    switch (Layer)
    {
    case 0:
        if (Column >= 0 && Column < TileMapComponent->TileMap->MapWidth && Row >= 0 && Row < TileMapComponent->TileMap->MapHeight)
            if (bEmptyOnly == false || TileMapComponent->GetTile(Column, Row, 0).GetTileIndex() == (int32)ETileType::Empty)
            {
                TileMapComponent->SetTile(Column, Row, 0, TileInfo);
            }
        break;
    default:
        break;
    }
}

TArray<UPaperSprite*> ACCTileMapActor::GetAllSpritesFromFolder(const FString& SpritePath)
{
    TArray<UPaperSprite*> Sprites;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName(*SpritePath));
    Filter.ClassPaths.Add(UPaperSprite::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        TObjectPtr<UObject> AssetObject = AssetData.GetAsset();
        UPaperSprite* Sprite = Cast<UPaperSprite>(AssetObject);
        if (Sprite)
        {
            Sprites.Add(Sprite);
        }
    }

    return Sprites;
}
