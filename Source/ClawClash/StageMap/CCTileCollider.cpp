// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileCollider.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"


UCCTileCollider::UCCTileCollider()
{
    OnComponentBeginOverlap.AddDynamic(this, &UCCTileCollider::BeginOverlap);
    SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    PrimaryComponentTick.bCanEverTick = true;
}

void UCCTileCollider::BeginPlay()
{
    Super::BeginPlay();
}

void UCCTileCollider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (NearbyActor != nullptr)
    {
        if (FMath::Abs(NearbyActor->GetActorLocation().X - ActorShownPos.X) > UCCManagers::GetInstance()->GetStageMapManager()->TileWidth
            || NearbyActor->GetActorLocation().Z > ActorShownPos.Z + 100 || NearbyActor->GetActorLocation().Z < ActorShownPos.Z - 100)
        {
            NearbyActor = nullptr;
            SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        }
        
    }
}

void UCCTileCollider::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
        if (OtherCharacter)
        {
            if (OtherCharacter->GetActorLocation().Z > GetRelativeLocation().Z + UCCManagers::GetInstance()->GetStageMapManager()->TileHeight - 1)
            {
                SetCollisionProfileName(TEXT("BlockAll"));
                NearbyActor = OtherActor;
                ActorShownPos = OtherActor->GetActorLocation();
            }
            else
            {
                SetCollisionProfileName(TEXT("OverlapAllDynamic"));
            }
        }
    }
}
