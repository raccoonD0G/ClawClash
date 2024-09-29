// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

ACCPaperNonPlayer::ACCPaperNonPlayer() : Super()
{
    GetCharacterMovement()->GravityScale = 1.0f;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACCPaperNonPlayer::BeginDestroy()
{
    OnNonPlayerCharacterDestroyed.Broadcast(this);

    Super::BeginDestroy();
}

void ACCPaperNonPlayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (GetActorLocation().X <= GetMaxLeftXPos() || GetActorLocation().X >= GetMaxRightXPos())
    {
        FVector NewVelocity = GetCharacterMovement()->Velocity;
        NewVelocity.X = 0.0f;
        NewVelocity.Y = 0.0f;

        GetCharacterMovement()->Velocity = NewVelocity;
    }
}

void ACCPaperNonPlayer::Init(float NewMaxLeftXPos, float NewMaxRightX)
{
    MaxLeftXPos = NewMaxLeftXPos;
    MaxRightXPos = NewMaxRightX;
}

void ACCPaperNonPlayer::FaceDirection(FVector Dir)
{
    FVector Scale;
    if (Dir.X >= GetActorLocation().X)
    {
        Scale = GetSprite()->GetComponentScale();
        Scale.X = FMath::Abs(Scale.X) * 1;
    }
    else
    {
        Scale = GetSprite()->GetComponentScale();
        Scale.X = FMath::Abs(Scale.X) * -1;
    }

    if (!IsRightStart)
    {
        Scale.X *= -1.0f;
    }

    GetSprite()->SetWorldScale3D(Scale);
}
void ACCPaperNonPlayer::SetPosition(FVector NewPos)
{
    NewPos.X = FMath::Clamp(NewPos.X, MaxLeftXPos, MaxRightXPos);
    SetActorLocation(NewPos);
}

void ACCPaperNonPlayer::StartMove()
{
}

void ACCPaperNonPlayer::EndMove()
{
}