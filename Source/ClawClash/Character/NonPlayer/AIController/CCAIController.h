// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CCAIController.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCAIController : public AAIController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    void RunAI();

    void StopAI();

    virtual void OnPossess(APawn* InPawn) override;

protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<class UBehaviorTree> BehaviorTree;

    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<UBlackboardData> BlackboardData;
	
};
