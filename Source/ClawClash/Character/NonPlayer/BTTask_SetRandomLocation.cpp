// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CCNonPlayerBB.h"
#include "CCPaperNonPlayer.h"

UBTTask_SetRandomLocation::UBTTask_SetRandomLocation()
{
}

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (ControlledPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    ACCPaperNonPlayer* ControlledCharacter = Cast<ACCPaperNonPlayer>(ControlledPawn);
    if (ControlledCharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    FBTMoveToTaskMemory* CurrentMem = (FBTMoveToTaskMemory*)NodeMemory;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (BlackboardComp)
    {
        CurrentMem->LeftXPos = ControlledCharacter->GetMaxLeftXPos();
        CurrentMem->RightXPos = ControlledCharacter->GetMaxRightXPos();

        float RandomX = FMath::RandRange(CurrentMem->LeftXPos, CurrentMem->RightXPos);
        FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
        BlackboardComp->SetValueAsVector(BBKEY_TATGETLOCATION, FVector(RandomX, CurrentLocation.Y, CurrentLocation.Z));

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
