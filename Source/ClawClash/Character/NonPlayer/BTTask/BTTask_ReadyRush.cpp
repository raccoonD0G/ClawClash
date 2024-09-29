// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Character/NonPlayer/BTTask/BTTask_ReadyRush.h"
#include "ClawClash/Interfaces/CCReadyRushable.h"
#include "AIController.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ReadyRush::UBTTask_ReadyRush()
{
	NodeName = "Ready Rush";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ReadyRush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FReadyRushMemory* ReadyRushMemory = (FReadyRushMemory*)NodeMemory;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ICCReadyRushable* ReadyRushable = Cast<ICCReadyRushable>(AIController->GetPawn());
    if (ReadyRushable == nullptr)
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
        return EBTNodeResult::Failed;
    }

    ReadyRushMemory->ReadyRushTime = ReadyRushable->GetReadyRushTime();
    ReadyRushMemory->CurrentReadyRushTime = 0.0f;

    ReadyRushable->FaceDirection(Player->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void UBTTask_ReadyRush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    FReadyRushMemory* ReadyRushMemory = (FReadyRushMemory*)NodeMemory;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    ICCReadyRushable* ReadyRushable = Cast<ICCReadyRushable>(AIController->GetPawn());
    if (ReadyRushable == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    if (ReadyRushMemory->ReadyRushTime <= ReadyRushMemory->CurrentReadyRushTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        ReadyRushMemory->CurrentReadyRushTime += DeltaSeconds;
    }
}

uint16 UBTTask_ReadyRush::GetInstanceMemorySize() const
{
    return sizeof(FReadyRushMemory);
}
