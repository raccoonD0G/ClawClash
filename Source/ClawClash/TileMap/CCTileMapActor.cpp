// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "CCTile.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACCTileMapActor::ACCTileMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the TileMapComponent and attach it to the Actor
	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
	RootComponent = TileMapComponent;
}

// Called when the game starts or when spawned
void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the TileMap
    TileMapComponent->CreateNewTileMap(TileMapWeidth, TileMapHeight, 256, 256, 1.0f, true);
    TileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);

    const UEnum* EnumPtr = StaticEnum<EGroundType>();
    int32 EnumLength = EnumPtr->NumEnums() - 1;
    GroundRatio.SetNum(EnumLength);

    GroundRatio[(int32)EGroundType::None] = 0.0f;
    GroundRatio[(int32)EGroundType::Basic] = 0.8f;
    GroundRatio[(int32)EGroundType::Waterside] = 0.0f;
    GroundRatio[(int32)EGroundType::Asphalt] = 0.0f;
    GroundRatio[(int32)EGroundType::Cave] = 0.0f;
    GroundRatio[(int32)EGroundType::Hill] = 0.01f;
    GroundRatio[(int32)EGroundType::HillSpace] = 0.0f;
    GroundRatio[(int32)EGroundType::Empty] = 0.0f;

    if (TileSet)
    {
        // Initialize TileInfo
        GroundTileArr.SetNum(EnumLength);
        for (int32 i = 0; i < GroundTileArr.Num(); i++)
        {
            GroundTileArr[i].TileSet = TileSet;
            GroundTileArr[i].PackedTileIndex = i;
        }

        // Fill Empty Tile
        for (int32 Row = 0; Row < TileMapHeight; Row ++)
        {
            for (int32 Column = 0; Column < TileMapWeidth; Column++)
            {
                SetTileIfPossible(Column, Row, 0, GroundTileArr[(int32)EGroundType::Empty]);
            }
        }

        for (int32 Row = (TileMapHeight / NumOfFloor) - 1; Row <= TileMapHeight; Row += TileMapHeight / NumOfFloor)
        {
            // Creat Necessary Hill
            if (Row != TileMapHeight - 1)
            {
                int32 DefaultHillColumn = FMath::RandRange(12, TileMapWeidth - 12);
                CreateGroundByType(EGroundType::Hill, DefaultHillColumn, Row);
            }
            
            for (int32 Column = 0; Column < TileMapWeidth; Column++)
            {   
                if (TileMapComponent->GetTile(Column, Row, 0).GetTileIndex() == (int32)EGroundType::Empty)
                {
                    int32 index = GetRandomIndexByProbability(GroundRatio);
                    if (index >= 0 && index < EnumLength)
                    {
                        EGroundType GroundTypeToPlace = (EGroundType)index;
                        CreateGroundByType(GroundTypeToPlace, Column, Row);
                    }
                }
            }
        }
        TileMapComponent->RebuildCollision();
    }
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

void ACCTileMapActor::CreateGroundByType(EGroundType CurrentType, int32 Column, int32 Row)
{
    
    switch (CurrentType)
    {
    case EGroundType::Basic:
        SetTileIfPossible(Column, Row, 0, GroundTileArr[(int32)CurrentType]);
        break;
    case EGroundType::Waterside:
        break;
    case EGroundType::Asphalt:
        break;
    case EGroundType::Cave:
        break;
    case EGroundType::Hill:
        CreatHill(Column, Row, 4);
        break;
    case EGroundType::Empty:
        break;
    }
}

void ACCTileMapActor::CreatHill(int32 Column, int32 Row, int32 StairLength)
{
    int32 LengthOfHill = FMath::RandRange(7, 12);
    SetTileIfPossible(Column, Row, 0, GroundTileArr[(int32)EGroundType::Hill]);
    for (int32 i = Column + 1; i < Column + LengthOfHill; i++)
    {
        SetTileIfPossible(i, Row, 0, GroundTileArr[(int32)EGroundType::HillSpace]);
        SetTileIfPossible(i, Row + TileMapHeight / NumOfFloor / 4, 0, GroundTileArr[(int32)EGroundType::Hill]);
    }
    for (int32 i = Column + 1 - StairLength; i < Column + 1; i++)
    {
        SetTileIfPossible(i, Row + (TileMapHeight / NumOfFloor) / 4 * 2, 0, GroundTileArr[(int32)EGroundType::Hill]);
    }
    for (int32 i = Column + LengthOfHill; i < Column + LengthOfHill + StairLength; i++)
    {
        SetTileIfPossible(i, Row + (TileMapHeight / NumOfFloor) / 4 * 2, 0, GroundTileArr[(int32)EGroundType::Hill]);
    }
    for (int32 i = Column + 1 - StairLength * 2; i < Column + 1 - StairLength; i++)
    {
        SetTileIfPossible(i, Row + (TileMapHeight / NumOfFloor) / 4 * 3, 0, GroundTileArr[(int32)EGroundType::Hill]);
    }
    for (int32 i = Column + LengthOfHill + StairLength; i < Column + LengthOfHill + StairLength * 2; i++)
    {
        SetTileIfPossible(i, Row + (TileMapHeight / NumOfFloor) / 4 * 3, 0, GroundTileArr[(int32)EGroundType::Hill]);
    }
    SetTileIfPossible(Column + LengthOfHill, Row, 0, GroundTileArr[(int32)EGroundType::Hill]);
}

void ACCTileMapActor::SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileToSet)
{
    switch (Layer)
    {
    case 0:
        if (Column >= 0 && Column < TileMapWeidth && Row >= 0 && Row < TileMapHeight) TileMapComponent->SetTile(Column, Row, 0, TileToSet);
        UE_LOG(LogTemp, Warning, TEXT("Setting tile at %d, %d"), Column, Row);
        break;
    default:
        break;
    }
}



