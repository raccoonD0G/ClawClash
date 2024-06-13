// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CCNonPlayerBB.h"
#include "CCPaperNonPlayer.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_SetRandomLocation::UBTTask_SetRandomLocation()
{
    NodeName = TEXT("Set Random Location");
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

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    float LeftXPos = ControlledCharacter->GetMaxLeftXPos();
    float RightXPos = ControlledCharacter->GetMaxRightXPos();
    float RandomX = FMath::RandRange(LeftXPos, RightXPos);
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    FVector RandomLocation(RandomX, CurrentLocation.Y, CurrentLocation.Z);

    BlackboardComp->SetValueAsVector(BBKEY_TATGETLOCATION, RandomLocation);
    ControlledCharacter->SetDest(RandomLocation);

    return EBTNodeResult::Succeeded;
}
