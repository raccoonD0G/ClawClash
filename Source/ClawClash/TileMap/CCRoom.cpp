// Fill out your copyright notice in the Description page of Project Settings.


#include "CCRoom.h"
#include "ClawClash/TileMap/CCFloor.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/Managers/CCStageMapManager.h"

void UCCRoom::Init(int32 InX, int32 InY, int32 InWidth, int32 InHeight)
{
    X = InX;
    Y = InY;
    Width = InWidth;
    Height = InHeight;
}

FVector2D UCCRoom::GetCenter() const
{
    return FVector2D(X + Width / 2.0f, Y + Height / 2.0f);
}

void UCCRoom::GenerateFloor()
{
    FloorStartX = FMath::RandRange(X, X + Width - UCCManagers::GetInstance()->GetStageMapManager()->MinFloorLength);
    FloorStartY = FMath::RandRange(Y + UCCManagers::GetInstance()->GetStageMapManager()->MinFloorHeight, Y + Height - UCCManagers::GetInstance()->GetStageMapManager()->MinFloorHeight);
    FloorInBox = NewObject<UCCFloor>();
    int32 FloorLength = FMath::RandRange(UCCManagers::GetInstance()->GetStageMapManager()->MinFloorLength, Width - FloorStartX + X);
    FloorInBox->Init(FloorLength, true);
}
