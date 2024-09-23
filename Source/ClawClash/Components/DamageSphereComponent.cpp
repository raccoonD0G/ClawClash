// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Components/DamageSphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"

UDamageSphereComponent::UDamageSphereComponent()
{
    // �⺻ ������ ����
    DamageAmount = 10.0f;

    // �浹 �̺�Ʈ ���ε�
    OnComponentBeginOverlap.AddDynamic(this, &UDamageSphereComponent::OnOverlapBegin);

    // �浹 �������� Ʈ���ŷ� ����
    SetCollisionProfileName(TEXT("Trigger"));
}

void UDamageSphereComponent::BeginPlay()
{
    Super::BeginPlay();

    SetSphereRadius(DamageRadius);
}

void UDamageSphereComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // ���� ���� ���� ���Ͱ� ��ȿ�ϰ� �ڽ��� �ƴ� ���
    if (OtherActor && OtherActor != GetOwner())
    {
        ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
        if (Player)
        {
            UGameplayStatics::ApplyDamage(Player, DamageAmount, nullptr, GetOwner(), nullptr);
        }
    }
}


