// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDetectorComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"

UPlayerDetectorComponent::UPlayerDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // SphereComponent 생성 및 초기화
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->InitSphereRadius(2000.0f); // 감지 반경 설정
    DetectionSphere->SetCollisionProfileName(TEXT("Trigger"));

    // Overlap 이벤트 바인딩
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerDetected);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerLeft);
}

void UPlayerDetectorComponent::BeginPlay()
{
    Super::BeginPlay();

    // 액터에 SphereComponent를 부착
    if (AActor* Owner = GetOwner())
    {
        DetectionSphere->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
}

bool UPlayerDetectorComponent::IsPlayerNearby()
{
    return !DetectedPlayers.IsEmpty();
}

void UPlayerDetectorComponent::OnPlayerDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어 감지 여부 확인 (캐릭터인지 체크)
    if (ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor))
    {
        if (DetectedPlayers.IsEmpty() == true)
        {
            OnPlayerFirstDetected.Broadcast(Player);
        }
        DetectedPlayers.Add(Player);

    }
}

void UPlayerDetectorComponent::OnPlayerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 플레이어가 감지 범위를 벗어났는지 확인
    if (ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor))
    {
        DetectedPlayers.Remove(Player);
        if (DetectedPlayers.IsEmpty() == true)
        {
            OnNoPlayerDetected.Broadcast();
        }
    }
}

