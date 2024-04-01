// Fill out your copyright notice in the Description page of Project Settings.


#include "CCGameInstance.h"
#include "CCTimerWidget.h"
#include "Blueprint/UserWidget.h"

UCCGameInstance::UCCGameInstance()
{
    RemainingGameTime = TotalGameTime;
}

void UCCGameInstance::Init()
{
    Super::Init();
}

void UCCGameInstance::OnStart()
{
    InitializeUI();
    StartGameTimer();
}

void UCCGameInstance::InitializeUI()
{
    if (IsValid(TimerWidgetClass))
    {
        TimerWidgetInstance = CreateWidget<UCCTimerWidget>(this, TimerWidgetClass);
        if (IsValid(TimerWidgetInstance))
        {
            TimerWidgetInstance->AddToViewport();
            TimerWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UCCGameInstance::StartGameTimer()
{
    GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UCCGameInstance::UpdateGameTimer, 1.0f, true);
}

void UCCGameInstance::UpdateGameTimer()
{
    RemainingGameTime--;

    Minute = RemainingGameTime / 60;
    Second = RemainingGameTime % 60;

    TimerWidgetInstance->UpdateTimerDisplay(Minute, Second);

    if (RemainingGameTime <= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);

        // GameResultCode Here
    }
}