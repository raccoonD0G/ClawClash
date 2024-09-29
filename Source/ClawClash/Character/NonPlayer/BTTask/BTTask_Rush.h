// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Rush.generated.h"

class ICCRushable;

USTRUCT()
struct FRushMemory
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<ICCRushable> Rushable;
	FVector Direction;
	float RushTime;
	float CurrentRushTime;
};
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UBTTask_Rush : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Rush();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};
