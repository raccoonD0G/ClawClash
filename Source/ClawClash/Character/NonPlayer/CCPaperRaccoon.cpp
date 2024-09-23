// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperRaccoon.h"
#include "PaperFlipbookComponent.h"
#include "ClawClash/Components/PlayerDetectorComponent.h"

ACCPaperRaccoon::ACCPaperRaccoon() : Super()
{
	PlayerDetectorComponent = CreateDefaultSubobject<UPlayerDetectorComponent>(TEXT("PlayerDetectorComponent"));
	IsRightStart = false;
}

void ACCPaperRaccoon::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentState(ERaccoonState::Idle);
}

void ACCPaperRaccoon::SetCurrentState(ERaccoonState NewState)
{
	switch (NewState)
	{
	case ERaccoonState::Idle:
		GetSprite()->SetFlipbook(IdleAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::Move:
		GetSprite()->SetFlipbook(MoveAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::ReadyAttack:
		GetSprite()->SetFlipbook(ReadyAttackAnimation);
		GetSprite()->Play();
		break;
	case ERaccoonState::Attack:
		GetSprite()->SetFlipbook(AttackAnimation);
		GetSprite()->Play();
		break;
	}

	CurrentState = NewState;
}
