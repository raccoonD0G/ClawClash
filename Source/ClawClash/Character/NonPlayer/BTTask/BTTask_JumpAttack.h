// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_JumpAttack.generated.h"

class ICCJumpAttackable;

USTRUCT()
struct FJumpAttackMemory
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ICCJumpAttackable> JumpAttakable;

	float JumpTime;
	float CurrentJumpTime;
	float AttackTime;
	float CurrentAttackTime;

	FVector Direction;
};
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UBTTask_JumpAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_JumpAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpPower;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

};
