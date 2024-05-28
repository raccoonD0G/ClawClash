// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetRandomLocation.generated.h"

USTRUCT()
struct FBTMoveToTaskMemory
{
    GENERATED_BODY()
public:
    float LeftXPos;
    float RightXPos;
};
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UBTTask_SetRandomLocation : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SetRandomLocation();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
