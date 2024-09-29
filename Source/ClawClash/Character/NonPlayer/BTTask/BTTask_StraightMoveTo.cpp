// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StraightMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClash/Character/NonPlayer/CCNonPlayerBB.h"
#include "ClawClash/Interfaces/CCMoveable.h"

UBTTask_StraightMoveTo::UBTTask_StraightMoveTo()
{
	NodeName = "Straight Move To";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StraightMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    ICCMoveable* Moveable = Cast<ICCMoveable>(AIController->GetPawn());
    if (!Moveable)
    {
        return EBTNodeResult::Failed;
    }

    Moveable->StartMove();

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_TATGETLOCATION);

    Moveable->FaceDirection(TargetLocation);

    FMoveMemory* MoveMemory = (FMoveMemory*)NodeMemory;

    MoveMemory->Moveable = Moveable;

    return EBTNodeResult::InProgress;
}

void UBTTask_StraightMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    FMoveMemory* MoveMemory = (FMoveMemory*)NodeMemory;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        MoveMemory->Moveable->EndMove();
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACCPaperNonPlayer* ControlledCharacter = Cast<ACCPaperNonPlayer>(AIController->GetPawn());
    if (!ControlledCharacter)
    {
        MoveMemory->Moveable->EndMove();
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        MoveMemory->Moveable->EndMove();
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_TATGETLOCATION);

    float DistanceToTarget = FMath::Abs(ControlledCharacter->GetActorLocation().X - TargetLocation.X);
    if (DistanceToTarget <= AcceptableRadius)
    {
        MoveMemory->Moveable->EndMove();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    ControlledCharacter->GetCharacterMovement()->AddInputVector(TargetLocation - ControlledCharacter->GetActorLocation());
}

uint16 UBTTask_StraightMoveTo::GetInstanceMemorySize() const
{
    return sizeof(FMoveMemory);;
}
