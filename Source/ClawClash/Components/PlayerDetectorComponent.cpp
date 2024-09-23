// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDetectorComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"

UPlayerDetectorComponent::UPlayerDetectorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // SphereComponent ���� �� �ʱ�ȭ
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->InitSphereRadius(2000.0f); // ���� �ݰ� ����
    DetectionSphere->SetCollisionProfileName(TEXT("Trigger"));

    // Overlap �̺�Ʈ ���ε�
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerDetected);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerLeft);
}

void UPlayerDetectorComponent::BeginPlay()
{
    Super::BeginPlay();

    // ���Ϳ� SphereComponent�� ����
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
    // �÷��̾� ���� ���� Ȯ�� (ĳ�������� üũ)
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
    // �÷��̾ ���� ������ ������� Ȯ��
    if (ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor))
    {
        DetectedPlayers.Remove(Player);
        if (DetectedPlayers.IsEmpty() == true)
        {
            OnNoPlayerDetected.Broadcast();
        }
    }
}

