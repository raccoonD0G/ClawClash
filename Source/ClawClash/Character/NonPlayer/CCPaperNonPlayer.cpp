// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCPaperNonPlayer::ACCPaperNonPlayer()
{
    GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

    GetCharacterMovement()->GravityScale = 1.0f;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACCPaperNonPlayer::BeginPlay()
{
    Super::BeginPlay();
}

float ACCPaperNonPlayer::GetMaxLeftXPos()
{
	return MaxLeftXPos;
}

float ACCPaperNonPlayer::GetMaxRightXPos()
{
	return MaxRightXPos;
}

void ACCPaperNonPlayer::SetMaxLeftXPos(float NewMaxLeftX)
{
    MaxLeftXPos = NewMaxLeftX;
}

void ACCPaperNonPlayer::SetMaxRightXPos(float NewMaxRightX)
{
    MaxRightXPos = NewMaxRightX;
}
