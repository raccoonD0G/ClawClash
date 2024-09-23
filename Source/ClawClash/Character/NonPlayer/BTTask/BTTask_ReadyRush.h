// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReadyRush.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UBTTask_ReadyRush : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ReadyRush();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
