// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileCollider.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/StageMap/CCTileMapActor.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "GameFramework/PlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"


UCCTileCollider::UCCTileCollider()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetCollisionProfileName(TEXT("CCPathTroughGround"));
}

void UCCTileCollider::Init(EFieldType NewFieldType)
{
    FieldType = NewFieldType;
}

void UCCTileCollider::BeginPlay()
{
    Super::BeginPlay();
    

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    Player = Cast<ACCPaperPlayer>(PlayerController->GetPawn());

    UCapsuleComponent* CapsuleComp = Player->GetCapsuleComponent();
    PlayerCapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
}

void UCCTileCollider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Player->GetActorLocation().Z - PlayerCapsuleHalfHeight > GetRelativeLocation().Z + UCCStageMapManager::GetStageMap()->GetTileHeight() / 2)
    {
        SetCollisionProfileName(TEXT("CCSolidGround"));
    }
    else
    {
        SetCollisionProfileName(TEXT("CCPathTroughGround"));
    }
}
