// Fill out your copyright notice in the Description page of Project Settings.


#include "CCStageMap.h"

#include "ClawClash/StageMap/StageMapParts/CCRoom.h"
#include "ClawClash/StageMap/StageMapParts/CCPlatform.h"

#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"

#include "ClawClash/StageMap/CCUnionFind.h"

const TArray<UCCPlatform*>& UCCStageMap::GetPlatformArr()
{
	return PlatformArr;
}

void UCCStageMap::SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth)
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
        Room1->Init(Space->GetTileMapPos(), SplitPoint, Space->Height);
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(FIntVector2(Space->GetTileMapPos().X + SplitPoint, Space->GetTileMapPos().Y), Space->Width - SplitPoint, Space->Height);
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
    else
    {
        int32 SplitPoint = FMath::RandRange(MinHeight, Space->Height - MinHeight);
        TObjectPtr<UCCRoom> Room1 = NewObject<UCCRoom>();
        Room1->Init(FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y), Space->Width, SplitPoint);
        TObjectPtr<UCCRoom> Room2 = NewObject<UCCRoom>();
        Room2->Init(FIntVector2(Space->GetTileMapPos().X, Space->GetTileMapPos().Y + SplitPoint), Space->Width, Space->Height - SplitPoint);
        SplitSpace(OutRooms, Room1, MinWidth, MinHeight, Depth - 1);
        SplitSpace(OutRooms, Room2, MinWidth, MinHeight, Depth - 1);
    }
}

void UCCStageMap::GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight)
{
    TObjectPtr<UCCRoom> InitialSpace = NewObject<UCCRoom>();
    InitialSpace->Init(FIntVector2(0, 0), MapWidth, MapHeight);
    int32 MaxDepth = 20; // 재귀 깊이 조정
    SplitSpace(OutRooms, InitialSpace, MinWidth, MinHeight, MaxDepth);
}

float UCCStageMap::CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2)
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


void UCCStageMap::GenerateMST()
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
        UE_LOG(LogTemp, Warning, TEXT("Connect Platform %d to Platform %d with Distance %f"), E.PlatformIndex1, E.PlatformIndex2, E.Weight);
        if (E.Weight >= 8.0f)
        {
            CreatePlatformsAlongEdge(E);
        }
    }
}

void UCCStageMap::CreatePlatformsAlongEdge(const FPlatformEdge& Edge)
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
        NewPlatform->Init(FIntVector2(NewPlatformPos.X, NewPlatformPos.Y), NewLength, false);
        PlatformArr.Add(NewPlatform);

        UE_LOG(LogTemp, Warning, TEXT("Created new platform at (%f, %f) with length %d"), NewPlatformPos.X, NewPlatformPos.Y, NewLength);
    }
}

void UCCStageMap::Init()
{
    StageMapManager = UCCManagers::GetInstance()->GetStageMapManager();
    TArray<UCCRoom*> RoomArr;
    GenerateRooms(RoomArr, StageMapManager->TileMapWidth, StageMapManager->TileMapHeight, StageMapManager->MinFloorHeight, StageMapManager->MinFloorLength);
    for (UCCRoom* Room : RoomArr)
    {
        PlatformArr.Add(Room->GeneratePlatform());
    }
    GenerateMST();
}
