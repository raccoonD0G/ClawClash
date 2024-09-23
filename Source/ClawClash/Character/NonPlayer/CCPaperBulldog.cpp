// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperBulldog.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "ClawClash/Components/PlayerDetectorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClash/Components/DamageSphereComponent.h"

ACCPaperBulldog::ACCPaperBulldog() : Super()
{
	PlayerDetectorComponent = CreateDefaultSubobject<UPlayerDetectorComponent>(TEXT("PlayerDetectorComponent"));
	MoveSpeed = 600.0f;
	RushMoveSpeed = 3000.0f;
	IsRightStart = false;
}

void ACCPaperBulldog::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentState(EBulldogState::Idle);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void ACCPaperBulldog::FaceDirection(FVector Dir)
{
	Super::FaceDirection(Dir);
}

void ACCPaperBulldog::SetCurrentState(EBulldogState NewState)
{
	switch (NewState)
	{
	case EBulldogState::Idle:
		GetSprite()->SetFlipbook(IdleAnimation);
		GetSprite()->Play();
		break;
	case EBulldogState::Move:
		GetSprite()->SetFlipbook(MoveAnimation);
		GetSprite()->Play();
		break;
	case EBulldogState::ReadyRush:
		GetSprite()->SetFlipbook(ReadyRushAnimation);
		GetSprite()->Play();
		break;
	case EBulldogState::Rush:
		GetSprite()->SetFlipbook(RushAnimation);
		GetSprite()->Play();
		break;
	}

	CurrentState = NewState;
}

void ACCPaperBulldog::ReadyRush()
{
	SetCurrentState(EBulldogState::ReadyRush);
}

bool ACCPaperBulldog::IsReadyRush()
{
	if (GetSprite()->GetFlipbook() != ReadyRushAnimation)
	{
		return false;
	}

	if (GetSprite()->GetPlaybackPositionInFrames() >= GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		return true;
	}

	return false;
}

void ACCPaperBulldog::StartRush()
{
	SetCurrentState(EBulldogState::Rush);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Trigger"));
	GetCharacterMovement()->MaxWalkSpeed = RushMoveSpeed;
	DamageSphereComponent = NewObject<UDamageSphereComponent>(this, UDamageSphereComponent::StaticClass());

	if (DamageSphereComponent)
	{
		DamageSphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		DamageSphereComponent->RegisterComponent();
	}
}

bool ACCPaperBulldog::IsRushEnd()
{
	if (GetSprite()->GetFlipbook() != RushAnimation)
	{
		return false;
	}

	if (GetSprite()->GetPlaybackPositionInFrames() >= GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		return true;
	}

	return false;
}

void ACCPaperBulldog::EndRush()
{
	SetCurrentState(EBulldogState::Idle);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CCNonPlayer"));
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	if (DamageSphereComponent)
	{
		DamageSphereComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageSphereComponent->DestroyComponent();
		DamageSphereComponent = nullptr;
	}
}

void ACCPaperBulldog::StartMove()
{
	Super::StartMove();
	SetCurrentState(EBulldogState::Move);
}

void ACCPaperBulldog::EndMove()
{
	Super::EndMove();
	SetCurrentState(EBulldogState::Idle);
}