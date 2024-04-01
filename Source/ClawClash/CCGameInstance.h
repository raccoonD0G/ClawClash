// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UCCGameInstance();
    virtual void Init() override;
    virtual void OnStart() override;

    UFUNCTION(BlueprintCallable, Category = "UI Management")
    void InitializeUI();

// TimerSection
public:
    void StartGameTimer();

protected:
    FTimerHandle GameTimerHandle;
    const int32 TotalGameTime = 180;
    int32 RemainingGameTime;

    void UpdateGameTimer();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Time")
    int32 Minute;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Time")
    int32 Second;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UCCTimerWidget> TimerWidgetClass;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UCCTimerWidget> TimerWidgetInstance;
};
