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

// Sets default values
ACCTileMapActor::ACCTileMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;

}

// Called when the game starts or when spawned
void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the TileMap
    FieldTileMapComponent->CreateNewTileMap(TileMapWeidth, TileMapHeight, 512, 512, 1.0f, true);
    FieldTileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);

    RootNode = NewObject<UCCBoxQuadTreeNode>(this);
    RootNode->Initialize(FVector2D(0, -TileMapHeight * 512), FVector2D(TileMapWeidth * 512, 0), 10);

    // Initialize FieldRatio
    int32 FieldTypeEnumLength = GetEnumLength(StaticEnum<EFieldType>());
    FieldRatio.SetNum(FieldTypeEnumLength);
    FieldRatio[(int32)EFieldType::BasicField] = 0.4f;
    FieldRatio[(int32)EFieldType::WatersideField] = 0.1f;
    FieldRatio[(int32)EFieldType::AsphaltField] = 0.1f;
    FieldRatio[(int32)EFieldType::CaveField] = 0.01f;
    FieldRatio[(int32)EFieldType::HillField] = 0.01f;
    FieldRatio[(int32)EFieldType::EmptyField] = 0.0f;

    // Initialize FieldInfo
    FieldInfoArr.SetNum(FieldTypeEnumLength);
    FieldInfoArr[(int32)EFieldType::HillField].MaxLength = 12;
    FieldInfoArr[(int32)EFieldType::HillField].MinLenght = 7;
    FieldInfoArr[(int32)EFieldType::WatersideField].MaxLength = 10;
    FieldInfoArr[(int32)EFieldType::WatersideField].MinLenght = 5;
    FieldInfoArr[(int32)EFieldType::CaveField].MaxLength = 5;
    FieldInfoArr[(int32)EFieldType::CaveField].MinLenght = 5;
    FieldInfoArr[(int32)EFieldType::AsphaltField].MaxLength = 10;
    FieldInfoArr[(int32)EFieldType::AsphaltField].MinLenght = 5;

    // Initialize Feature
    FieldInfoArr[(int32)EFieldType::WatersideField].FeatureRatio.SetNum(GetEnumLength(StaticEnum<EWaterSideFeature>()));
    FieldInfoArr[(int32)EFieldType::WatersideField].FeatureRatio[(int32)EWaterSideFeature::Weed0Feature] = 0.3f;

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
            for (int32 Column = 0; Column < TileMapWeidth; Column++)
            {
                SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Empty], false);
            }
        }

        for (int32 Row = (TileMapHeight / NumOfFloor) - 1; Row <= TileMapHeight; Row += TileMapHeight / NumOfFloor)
        {
            // Creat Necessary Hill
            if (Row != TileMapHeight - 1)
            {
                int32 DefaultHillColumn = FMath::RandRange(FieldInfoArr[(int32)EFieldType::HillField].MaxLength, TileMapWeidth - FieldInfoArr[(int32)EFieldType::HillField].MaxLength);
                CreateFieldByType(EFieldType::HillField, DefaultHillColumn, Row);
            }
            
            for (int32 Column = 0; Column < TileMapWeidth; Column++)
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
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)CurrentType]);
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

void ACCTileMapActor::CreatHill(int32 Column, int32 Row, int32 StairLength)
{
    if (Row == TileMapHeight - 1)
    {
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Basic]);
        return;
    }

    int32 LengthOfHill = FMath::RandRange(FieldInfoArr[(int32)EFieldType::HillField].MinLenght, FieldInfoArr[(int32)EFieldType::HillField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfHill) == false)
    {
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Basic]);
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
    int32 LengthOfWaterSide = FMath::RandRange(FieldInfoArr[(int32)EFieldType::WatersideField].MinLenght, FieldInfoArr[(int32)EFieldType::WatersideField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfWaterSide) == false)
    {
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Basic]);
        return;
    }
    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::WatersideLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfWaterSide - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Waterside]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfWaterSide - 1, Row, 0, TileInfoArr[(int32)ETileType::WatersideRight]);

    PlaceSpritesOnTileMap(FieldTileMapComponent->TileMap, FVector2D(Column, Row), LengthOfWaterSide, WeedSprite);
}

void ACCTileMapActor::CreateCave(int32 Column, int32 Row)
{
    int32 LengthOfWaterSide = FMath::RandRange(FieldInfoArr[(int32)EFieldType::CaveField].MinLenght, FieldInfoArr[(int32)EFieldType::CaveField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfWaterSide) == false)
    {
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Basic]);
        return;
    }

    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::CaveLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfWaterSide - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Cave]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfWaterSide - 1, Row, 0, TileInfoArr[(int32)ETileType::CaveRight]);
}

void ACCTileMapActor::PlaceSpritesOnTileMap(TObjectPtr<UPaperTileMap> TileMap, FVector2D StartingTile, int32 OffsetTiles, TObjectPtr<UPaperSprite> SpriteToPlace, bool bAllowOverlap)
{
    OffsetTiles--;

    if (!TileMap || !SpriteToPlace)
    {
        return;
    }

    FVector2D TileSize(TileMap->TileWidth, TileMap->TileHeight);
    FVector2D StartLocalPos = StartingTile * TileSize;
    FVector2D EndLocalPos = StartLocalPos + FVector2D(OffsetTiles * TileSize.X, 0);

    FTransform ActorTransform = GetActorTransform();
    FVector StartWorldPos = ActorTransform.TransformPosition(FVector(StartLocalPos, 0.0f));
    FVector EndWorldPos = ActorTransform.TransformPosition(FVector(EndLocalPos, 0.0f));

    for (float i = StartWorldPos.X; i <= EndWorldPos.X; i += TileSize.X / 2)
    {
        FVector WorldPos(i, 0.0f, -StartWorldPos.Y + 1.5 * TileSize.Y);
        FBox2D BoxForSprite(FVector2D(WorldPos.X, WorldPos.Z), FVector2D(WorldPos.X + SpriteToPlace->GetSourceSize().X, WorldPos.Z + SpriteToPlace->GetSourceSize().Y));

        if (bAllowOverlap || !RootNode->IsColliding(BoxForSprite))
        {
            TObjectPtr<UPaperSpriteComponent> NewSprite = NewObject<UPaperSpriteComponent>(this);
            if (NewSprite)
            {
                NewSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                NewSprite->RegisterComponent();
                NewSprite->SetVisibility(true);
                NewSprite->SetSprite(SpriteToPlace);
                NewSprite->SetRelativeLocation(WorldPos);
                NewSprite->SetCollisionProfileName(TEXT("NoCollision"));

                RootNode->Insert(BoxForSprite);
                FeatureSpriteComponentArr.Add(NewSprite);
            }
        }
    }
}


void ACCTileMapActor::CreateAsphalt(int32 Column, int32 Row)
{
    int32 LengthOfWaterSide = FMath::RandRange(FieldInfoArr[(int32)EFieldType::AsphaltField].MinLenght, FieldInfoArr[(int32)EFieldType::AsphaltField].MaxLength);
    if (CheckAllEmpty(Column, Row, LengthOfWaterSide) == false)
    {
        SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::Basic]);
        return;
    }

    SetTileIfPossible(FieldTileMapComponent, Column, Row, 0, TileInfoArr[(int32)ETileType::AsphaltLeft]);
    for (int32 i = Column + 1; i < Column + LengthOfWaterSide - 1; i++)
    {
        SetTileIfPossible(FieldTileMapComponent, i, Row, 0, TileInfoArr[(int32)ETileType::Asphalt]);
    }
    SetTileIfPossible(FieldTileMapComponent, Column + LengthOfWaterSide - 1, Row, 0, TileInfoArr[(int32)ETileType::AsphaltRight]);
}

bool ACCTileMapActor::CheckAllEmpty(int32 Column, int32 Row, int32 Length)
{
    for (int32 i = Column; i < Column + Length; i++)
    {
        if (i >= 0 && i < TileMapWeidth && Row >= 0 && Row < TileMapHeight)
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
        UE_LOG(LogTemp, Warning, TEXT("Setting tile at %d, %d"), Column, Row);
        break;
    default:
        break;
    }
}



