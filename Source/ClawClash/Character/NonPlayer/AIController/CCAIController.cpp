// Fill out your copyright notice in the Description page of Project Settings.


#include "CCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ClawClash/Components/AIActorComponent.h"

void ACCAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ACCAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BlackboardData, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BehaviorTree);
		ensure(RunResult);
	}
}

void ACCAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ACCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
	TSet<UActorComponent*> Components = GetComponents();

	for (UActorComponent* Component : Components)
	{
		UAIActorComponent* AIComponent = Cast<UAIActorComponent>(Component);
		if (AIComponent != nullptr)
		{
			AIComponent->OnPossess(InPawn);
		}
	}
}