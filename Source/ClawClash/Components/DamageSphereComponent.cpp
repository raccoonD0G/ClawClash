// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Components/DamageSphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"

UDamageSphereComponent::UDamageSphereComponent()
{
    // 기본 데미지 설정
    DamageAmount = 10.0f;

    // 충돌 이벤트 바인딩
    OnComponentBeginOverlap.AddDynamic(this, &UDamageSphereComponent::OnOverlapBegin);

    // 충돌 프로필을 트리거로 설정
    SetCollisionProfileName(TEXT("Trigger"));
}

void UDamageSphereComponent::BeginPlay()
{
    Super::BeginPlay();

    SetSphereRadius(DamageRadius);
}

void UDamageSphereComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 범위 내에 들어온 액터가 유효하고 자신이 아닌 경우
    if (OtherActor && OtherActor != GetOwner())
    {
        ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
        if (Player)
        {
            UGameplayStatics::ApplyDamage(Player, DamageAmount, nullptr, GetOwner(), nullptr);
        }
    }
}


