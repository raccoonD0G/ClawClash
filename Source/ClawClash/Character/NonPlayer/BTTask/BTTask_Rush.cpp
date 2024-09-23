// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Character/NonPlayer/BTTask/BTTask_Rush.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"
#include "AIController.h"
#include "ClawClash/Interfaces/CCRushable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_Rush::UBTTask_Rush()
{
    NodeName = "Rush";
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Rush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FRushMemory* RushMemory = (FRushMemory*)NodeMemory;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* ControlledCharacter = Cast<ACharacter>(AIController->GetPawn());
    if (ControlledCharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ICCRushable* Rushable = Cast<ICCRushable>(ControlledCharacter);
    if (Rushable == nullptr)
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
        RushMemory->Rushable->EndRush();
        return EBTNodeResult::Failed;
    }

    FVector PlayerDir = Player->GetActorLocation() - ControlledCharacter->GetActorLocation();
    FVector Direction = FVector::Zero();
    if (PlayerDir.X > 0)
    {
        Direction.X = 1.0f;
    }
    else
    {
        Direction.X = -1.0f;
    }

    RushMemory->Rushable = Rushable;
    RushMemory->Direction = Direction;

    Rushable->StartRush();
    Rushable->FaceDirection(Player->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void UBTTask_Rush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    FRushMemory* RushMemory = (FRushMemory*)NodeMemory;

    if (RushMemory->Rushable->IsRushEnd())
    {
        RushMemory->Rushable->EndRush();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        RushMemory->Rushable->EndRush();
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    ACharacter* ControlledCharacter = Cast<ACharacter>(AIController->GetPawn());
    if (ControlledCharacter == nullptr)
    {
        RushMemory->Rushable->EndRush();
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    float Speed = ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed;
    ControlledCharacter->SetActorLocation(ControlledCharacter->GetActorLocation() + RushMemory->Direction * Speed * DeltaSeconds);
}

uint16 UBTTask_Rush::GetInstanceMemorySize() const
{
    return sizeof(FRushMemory);
}
