// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StraightMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CCPaperNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CCNonPlayerBB.h"

UBTTask_StraightMoveTo::UBTTask_StraightMoveTo()
{
	NodeName = "Straight Move To";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StraightMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    return EBTNodeResult::InProgress;
}

void UBTTask_StraightMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACCPaperNonPlayer* ControlledCharacter = Cast<ACCPaperNonPlayer>(AIController->GetPawn());
    if (!ControlledCharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    FVector TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_TATGETLOCATION);

    float DistanceToTarget = FMath::Abs(ControlledCharacter->GetActorLocation().X - TargetLocation.X);
    if (DistanceToTarget <= AcceptableRadius)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector Direction = (TargetLocation - ControlledCharacter->GetActorLocation()).GetSafeNormal();
    float Speed = ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed;
    FVector NewLocation = ControlledCharacter->GetActorLocation() + Direction * Speed * DeltaSeconds;

    ControlledCharacter->SetActorLocation(NewLocation);
}
