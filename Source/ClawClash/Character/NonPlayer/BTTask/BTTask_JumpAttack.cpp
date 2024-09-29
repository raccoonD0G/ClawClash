// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Character/NonPlayer/BTTask/BTTask_JumpAttack.h"
#include "ClawClash/Interfaces/CCJumpAttackable.h"
#include "AIController.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_JumpAttack::UBTTask_JumpAttack()
{
	NodeName = "Jump Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FJumpAttackMemory* JumpAttackMemory = (FJumpAttackMemory*)NodeMemory;

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = Cast<ACharacter>(Controller->GetPawn());
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}

	ICCJumpAttackable* JumpAttackable = Cast<ICCJumpAttackable>(Controller->GetPawn());
	if (!JumpAttackable)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(BlackboardComp->GetValueAsObject(TEXT("NearbyPlayer")));
	if (Player == nullptr)
	{
		JumpAttackable->EndAttack();
		return EBTNodeResult::Failed;
	}

	FVector PlayerDir = Player->GetActorLocation() - Character->GetActorLocation();
	FVector Direction = FVector::Zero();
	if (PlayerDir.X > 0)
	{
		Direction.X = 1.0f;
	}
	else
	{
		Direction.X = -1.0f;
	}

	JumpAttackable->FaceDirection(Player->GetActorLocation());

	JumpAttackMemory->JumpAttakable = JumpAttackable;
	JumpAttackMemory->JumpTime = JumpAttackable->GetJumpTime();
	JumpAttackMemory->CurrentJumpTime = 0.0f;
	JumpAttackMemory->AttackTime = JumpAttackable->GetAttackTime();
	JumpAttackMemory->CurrentAttackTime = 0.0f;
	JumpAttackMemory->Direction = Direction;

	Character->LaunchCharacter(FVector(0, 0, JumpPower), true, true);

	JumpAttackable->StartJump();

	return EBTNodeResult::InProgress;
}

void UBTTask_JumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FJumpAttackMemory* JumpAttackMemory = (FJumpAttackMemory*)NodeMemory;

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACCPaperNonPlayer* NonPlayer = Cast<ACCPaperNonPlayer>(Controller->GetPawn());
	if (!NonPlayer)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (JumpAttackMemory->JumpTime > JumpAttackMemory->CurrentJumpTime)
	{
		JumpAttackMemory->CurrentJumpTime += DeltaSeconds;
		if (JumpAttackMemory->JumpTime <= JumpAttackMemory->CurrentJumpTime)
		{
			JumpAttackMemory->JumpAttakable->EndJump();
			JumpAttackMemory->JumpAttakable->StartAttack();
			NonPlayer->LaunchCharacter(FVector(JumpAttackMemory->Direction.X * JumpPower, 0, -JumpPower), true, true);
		}
	}
	else
	{
		if (JumpAttackMemory->AttackTime > JumpAttackMemory->CurrentAttackTime)
		{
			JumpAttackMemory->CurrentAttackTime += DeltaSeconds;
		}
		else
		{
			JumpAttackMemory->JumpAttakable->EndAttack();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}

uint16 UBTTask_JumpAttack::GetInstanceMemorySize() const
{
	return sizeof(FJumpAttackMemory);
}
