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

#include "ClawClash/StageMap/StageMapParts/CCRoom.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"

#include "ClawClash/StageMap/CCUnionFind.h"
#include "ClawClash/StageMap/CCTileCollider.h"

ACCTileMapActor::ACCTileMapActor()
{
	PrimaryActorTick.bCanEverTick = true;
    FieldTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("FieldTileMapComponent"));
	RootComponent = FieldTileMapComponent;
    BackgroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
    BackgroundComponent->SetupAttachment(RootComponent);
    //TilePlacer = CreateDefaultSubobject<UCCTilePlacer>(TEXT("TileMapPlacer"));
    SpritePlacer = CreateDefaultSubobject<UCCSpritePlacer>(TEXT("SpritePlacer"));

    FieldTileMapComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

int32 ACCTileMapActor::GetTileMapWidth()
{
    return TileMapWidth;
}

int32 ACCTileMapActor::GetTileMapHeight()
{
    return TileMapHeight;
}

int32 ACCTileMapActor::GetTileWidth()
{
    return TileWidth;
}

int32 ACCTileMapActor::GetTileHeight()
{
    return TileHeight;
}

int32 ACCTileMapActor::GetMinRoomHeight()
{
    return MinRoomHeight;
}

int32 ACCTileMapActor::GetMinRoomWidth()
{
    return MinRoomWidth;
}

/*
void ACCTileMapActor::InitializeTileMap()
{
    if (TilePlacer && FieldTileMapComponent && FieldTileSet)
    {
        TilePlacer->InitializeTileSet(FieldTileSet);
        TilePlacer->InitializeTileMap(FieldTileMapComponent, FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
        TilePlacer->CreatAllField(FieldTileMapComponent);
        TilePlacer->FillInBasic(FieldTileMapComponent);
    }
}
*/


void ACCTileMapActor::InitializeBackground()
{
    int32 Index = FMath::RandRange(0, BackGroundSpriteArr.Num() - 1);
    BackGroundSprite = BackGroundSpriteArr[Index];

    if (BackGroundSprite && BackgroundComponent)
    {
        float TileMapWidthInPixels =  TileMapWidth * 512;
        float TileMapHeightInPixels = TileMapHeight * 512;

        BackgroundComponent->SetSprite(BackGroundSprite);
        BackgroundComponent->SetRelativeLocation(FVector(Player->GetActorLocation().X, UCCStageMapManager::GetInstance()->BackgroundY, Player->GetActorLocation().Z));
        BackgroundComponent->SetRelativeScale3D(FVector(TileMapWidthInPixels / (2.0 * BackgroundComponent->GetSprite()->GetRenderBounds().BoxExtent.X * 2), 1.0f, TileMapWidthInPixels / (2.0 * BackgroundComponent->GetSprite()->GetRenderBounds().BoxExtent.X * 2)));
        BackgroundComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
}

void ACCTileMapActor::PlaceFieldSprites()
{
    SpritePlacer->InitializeSprite(FieldTileMapComponent, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
}

void ACCTileMapActor::BeginPlay()
{
    Super::BeginPlay();

    if (UCCStageMapManager::GetStageMap() == nullptr || !UCCStageMapManager::GetStageMap()->IsValidLowLevel())
    {
        UCCStageMapManager::SetStageMap(this);
    }
    else
    {
        Destroy();
        return;
    }

    if (FieldTileMapComponent && FieldTileSet)
    {
        UCCStageMapManager::GetInstance()->InitializeTileSet(FieldTileSet);
        InitializeTileMap(FieldTileSet, TileMapHeight, TileMapWidth, TileWidth, TileHeight);
    }

    TArray<UCCRoom*> RoomArr;
    GenerateRooms(RoomArr, TileMapWidth, TileMapHeight, MinRoomHeight, MinRoomWidth);
    for (UCCRoom* Room : RoomArr)
    {
        PlatformArr.Add(Room->GeneratePlatform());
    }
    GenerateMST();

    // Initialize Player
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    FieldTileMapComponent->RebuildCollision();
    PlaceFieldSprites();
    InitializeBackground();
}

// Called every frame
void ACCTileMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    BackgroundComponent->SetRelativeLocation(FVector(LastBackgroundX + ((Player->GetActorLocation().X - LastPlayerX) / 1.2), UCCStageMapManager::GetInstance()->BackgroundY, LastBackgroundZ + ((Player->GetActorLocation().Z - LastPlayerZ) / 1.2)));
    LastPlayerX = Player->GetActorLocation().X;
    LastPlayerZ = Player->GetActorLocation().Z;
    LastBackgroundX = BackgroundComponent->GetRelativeLocation().X;
    LastBackgroundZ = BackgroundComponent->GetRelativeLocation().Z;

}

const TArray<UCCPlatform*>& ACCTileMapActor::GetPlatformArr()
{
    return PlatformArr;
}

void ACCTileMapActor::SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth)
{
    if (Depth <= 0 || Space->Width <= MinWidth * 2 || Space->Height <= MinHeight * 2)
    {
        OutRooms.Add(Space);
        return;
    }

    bool SplitHorizontally = (Space->Width > Space->Height);
    if (SplitHorizontally)
    {
        int32 SplitPoint = FMath::RandRange(MinWidth, Space->Width - MinWidth);
        TObjectPtr<UCCRoom> Room1 = NewObject<UCCRoom>();
        Room1->Init(this, Space->GetTileMapPos(), SplitPoint, Space->Height);
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(this, FIntVector2(Space->GetTileMapPos().X + SplitPoint, Space->GetTileMapPos().Y), Space->Width - SplitPoint, Space->Height);
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
    else
    {
        int32 SplitPoint = FMath::RandRange(MinHeight, Space->Height - MinHeight);
        TObjectPtr<UCCRoom> Room1 = NewObject<UCCRoom>();
        Room1->Init(this, FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y), Space->Width, SplitPoint);
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(this, FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y + SplitPoint), Space->Width, Space->Height - SplitPoint);
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
}

void ACCTileMapActor::GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight)
{
    TObjectPtr<UCCRoom> InitialSpace = NewObject<UCCRoom>();
    InitialSpace->Init(this, FIntVector2(0, 0), MapWidth, MapHeight);
    int32 MaxDepth = 20; // 재귀 깊이 조정
    SplitSpace(OutRooms, InitialSpace, MinWidth, MinHeight, MaxDepth);
}

float ACCTileMapActor::CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2)
{

    FVector2D StartPos0 = FVector2D(Platform0.GetStartPos().X, Platform0.GetStartPos().Y);
    FVector2D EndPos0 = FVector2D(Platform0.GetEndPos().X, Platform0.GetEndPos().Y);
    FVector2D StartPos1 = FVector2D(Platform1.GetStartPos().X, Platform1.GetStartPos().Y);
    FVector2D EndPos1 = FVector2D(Platform1.GetEndPos().X, Platform1.GetEndPos().Y);

    if (StartPos0.X >= EndPos1.X)
    {
        Pos1 = Platform0.GetStartPos();
        Pos2 = Platform1.GetStartPos();
        return  FVector2D::Distance(StartPos0, StartPos1);
    }
    else if (EndPos0.X <= StartPos1.X)
    {
        Pos1 = Platform0.GetEndPos();
        Pos2 = Platform1.GetStartPos();
        return FVector2D::Distance(EndPos0, StartPos1);
    }
    else if (StartPos0.X <= EndPos1.X && StartPos0.X >= StartPos1.X)
    {
        Pos1 = Platform0.GetStartPos();
        Pos2 = FIntVector2(Platform0.GetStartPos().X, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (EndPos0.X <= EndPos1.X && EndPos0.X >= StartPos1.X)
    {
        Pos1 = Platform0.GetEndPos();
        Pos2 = FIntVector2(Platform0.GetEndPos().X, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (StartPos0.X <= StartPos1.X && EndPos0.X >= EndPos1.X)
    {
        Pos1 = FIntVector2((Platform1.GetStartPos().X + Platform1.GetEndPos().X) / 2, Platform1.GetStartPos().Y);
        Pos2 = FIntVector2((Platform1.GetStartPos().X + Platform1.GetEndPos().X) / 2, Platform0.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }
    else if (StartPos0.X >= StartPos1.X && EndPos0.X <= EndPos1.X)
    {
        Pos1 = FIntVector2((Platform0.GetStartPos().X + Platform0.GetEndPos().X) / 2, Platform0.GetStartPos().Y);
        Pos2 = FIntVector2((Platform0.GetStartPos().X + Platform0.GetEndPos().X) / 2, Platform1.GetStartPos().Y);
        return FMathf::Abs(StartPos0.Y - StartPos1.Y);
    }

    return -1.0f;
}


void ACCTileMapActor::GenerateMST()
{
    if (PlatformArr.Num() < 2)
    {
        return; // 플랫폼이 두 개 미만일 경우 MST를 생성할 수 없음
    }

    TArray<FPlatformEdge> Edges;

    // 모든 플랫폼 쌍의 거리를 계산하여 간선 목록을 생성
    for (int32 i = 0; i < PlatformArr.Num(); ++i)
    {
        for (int32 j = i + 1; j < PlatformArr.Num(); ++j)
        {
            FIntVector2 Pos1;
            FIntVector2 Pos2;
            float Distance = CalculatePlatformDistance(*PlatformArr[i], *PlatformArr[j], Pos1, Pos2);
            Edges.Add(FPlatformEdge(i, j, Distance, Pos1, Pos2));
        }
    }

    // 간선을 가중치(거리)에 따라 정렬
    Edges.Sort();

    // Union-Find 구조체 초기화
    TObjectPtr<UCCUnionFind> UnionFind;
    UnionFind = NewObject<UCCUnionFind>();
    UnionFind->Init(PlatformArr.Num());

    TArray<FPlatformEdge> MST;
    MST.Reserve(PlatformArr.Num() - 1);

    // 크루스칼 알고리즘을 이용하여 MST 생성
    for (const FPlatformEdge& E : Edges)
    {
        if (UnionFind->Find(E.PlatformIndex1) != UnionFind->Find(E.PlatformIndex2))
        {
            MST.Add(E);
            UnionFind->Union(E.PlatformIndex1, E.PlatformIndex2);

            // MST의 간선 수가 (N-1)개가 되면 완료
            if (MST.Num() == PlatformArr.Num() - 1)
            {
                break;
            }
        }
    }

    // MST 결과를 사용하여 플랫폼을 연결
    for (const FPlatformEdge& E : MST)
    {
        if (E.Weight >= 8.0f)
        {
            CreatePlatformsAlongEdge(E);
        }
    }
}

void ACCTileMapActor::CreatePlatformsAlongEdge(const FPlatformEdge& Edge)
{
    FVector2D FloatPos1(Edge.Pos1.X, Edge.Pos1.Y);
    FVector2D FloatPos2(Edge.Pos2.X, Edge.Pos2.Y);

    FVector2D Direction = (FloatPos2 - FloatPos1).GetSafeNormal();
    float Distance = Edge.Weight;
    int32 NumOfPlatform = Distance / 8;

    for (float CurrentDistance = Distance / (NumOfPlatform + 1); CurrentDistance < Distance; CurrentDistance += Distance / (NumOfPlatform + 1))
    {
        FVector2D NewPlatformPos = FloatPos1 + Direction * CurrentDistance;
        int32 NewLength = FMath::RandRange(6, 8);
        NewPlatformPos.X -= NewLength / 2;
        // 새 플랫폼 생성
        TObjectPtr<UCCPlatform> NewPlatform = NewObject<UCCPlatform>();
        NewPlatform->Init(this, FIntVector2(NewPlatformPos.X, NewPlatformPos.Y), NewLength);
        PlatformArr.Add(NewPlatform);
    }
}

void ACCTileMapActor::InitializeTileMap(UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight)
{
    TileMapHeight = Rows;
    TileMapWidth = Columns;
    TileWidth = NewTileWidth;
    TileHeight = NewTileHeight;

    if (FieldTileMapComponent && TileSet)
    {
        FieldTileMapComponent->CreateNewTileMap(Columns, Rows, TileWidth, TileHeight, 1.0f, true);
        FieldTileMapComponent->TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
        FieldTileMapComponent->SetRelativeLocation(FVector(0, UCCStageMapManager::GetInstance()->FieldTileY, 0));

        for (int32 Row = 0; Row < Rows; ++Row)
        {
            for (int32 Column = 0; Column < Columns; ++Column)
            {
                FPaperTileInfo TileInfo;
                TileInfo.TileSet = TileSet;
                TileInfo.PackedTileIndex = (int32)ETileType::None;
                SetTileIfPossible(Column, Row, 0, TileInfo, false);
            }
        }
    }
}

bool ACCTileMapActor::SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileInfo, bool bEmptyOnly)
{
    if (FieldTileMapComponent)
    {
        if (Column >= 0 && Column < FieldTileMapComponent->TileMap->MapWidth && Row >= 0 && Row < FieldTileMapComponent->TileMap->MapHeight)
        {
            if (!bEmptyOnly || FieldTileMapComponent->GetTile(Column, Row, Layer).GetTileIndex() == (int32)ETileType::None)
            {
                FieldTileMapComponent->SetTile(Column, Row, Layer, TileInfo);
                return true;
            }
        }
    }
    return false;
}

void ACCTileMapActor::SetupTileColliders(int32 Column, int32 Row, int32 ColliderLength, EFieldType NewFieldType)
{
    int32 TileSize = UCCStageMapManager::GetStageMap()->GetTileHeight();

    UCCTileCollider* CustomCollider = NewObject<UCCTileCollider>(FieldTileMapComponent->GetOwner());
    CustomCollider->Init(NewFieldType);
    ColliderArr.Add(CustomCollider);
    CustomCollider->AttachToComponent(FieldTileMapComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CustomCollider->SetBoxExtent(FVector(TileSize / 2 * ColliderLength, 1000.0f, TileSize / 2));
    CustomCollider->SetRelativeLocation(FVector(Column * TileSize + TileSize / 2 * ColliderLength, 0, -Row * TileSize));

    CustomCollider->RegisterComponent();
}

void ACCTileMapActor::CreatAllField(UPaperTileMapComponent* TileMapComponent)
{
    for (UCCPlatform* Platform : UCCStageMapManager::GetInstance()->GetStageMap()->GetPlatformArr())
    {
        for (UCCField* Field : Platform->GetFieldArr())
        {
            Field->CreateTile();
        }
    }
}