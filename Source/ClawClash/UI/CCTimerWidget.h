// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void UpdateTimerDisplay(int32 Minutes, int32 Seconds);

    UFUNCTION(BlueprintImplementableEvent, Category = "Timer")
    void SetTimerText(int32 Minutes, int32 Seconds);
};
