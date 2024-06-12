// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StraightMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UBTTask_StraightMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_StraightMoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptableRadius = 100.0f;
	
};
