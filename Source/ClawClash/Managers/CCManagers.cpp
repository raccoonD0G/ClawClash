// Fill out your copyright notice in the Description page of Project Settings.


#include "CCManagers.h"
#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"
#include "CCManagers.h"


TObjectPtr<UCCManagers> UCCManagers::Instance;


UCCManagers* UCCManagers::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = NewObject<UCCManagers>();
        Instance->AddToRoot();
    }
    return Instance;
}

void UCCManagers::SetInstanceNull()
{
    Instance = nullptr;
}

UCCStageMapManager* UCCManagers::GetStageMapManager()
{
    if (StageMapManager == NULL)
    {
        StageMapManager = NewObject<UCCStageMapManager>(this);
        StageMapManager->Init();
    }
    return StageMapManager;
}

UCCSpawnManager* UCCManagers::GetSpawnManager()
{
    if (SpawnManager == NULL)
    {
        SpawnManager = NewObject<UCCSpawnManager>(this);
        SpawnManager->Init();
    }
    return SpawnManager;
}

int32 UCCManagers::GetRandomIndexByProbability(const TArray<float>& Probabilities)
{
    if (Probabilities.Num() == 0)
    {
        return -1;
    }

    TArray<float> CumulativeProbabilities;
    float TotalProbability = 0.f;

    for (float Probability : Probabilities)
    {
        TotalProbability += Probability;
        CumulativeProbabilities.Add(TotalProbability);
    }

    float RandomNumber = FMath::FRandRange(0.0f, TotalProbability);

    int32 Low = 0;
    int32 High = CumulativeProbabilities.Num() - 1;

    while (Low < High)
    {
        int32 Mid = (Low + High) / 2;
        if (RandomNumber <= CumulativeProbabilities[Mid])
        {
            High = Mid;
        }
        else
        {
            Low = Mid + 1;
        }
    }

    return Low;
}


int32 UCCManagers::GetRandomIndexByProbability(const TArray<FCCFeatureInfo>& Probabilities)
{
    if (Probabilities.Num() == 0)
    {
        return -1;
    }

    TArray<float> CumulativeProbabilities;
    float TotalProbability = 0.f;

    for (const FCCFeatureInfo& Probability : Probabilities)
    {
        TotalProbability += Probability.FeatureRatio;
        CumulativeProbabilities.Add(TotalProbability);
    }

    float RandomNumber = FMath::FRandRange(0.0f, TotalProbability);

    int32 Low = 0;
    int32 High = CumulativeProbabilities.Num() - 1;

    while (Low < High)
    {
        int32 Mid = (Low + High) / 2;
        if (RandomNumber <= CumulativeProbabilities[Mid])
        {
            High = Mid;
        }
        else
        {
            Low = Mid + 1;
        }
    }

    return Low;
}

int32 UCCManagers::GetEnumLength(UEnum* TargetEnum)
{
    return TargetEnum->NumEnums() - 1;
}

TArray<int32> UCCManagers::DecomposeNumberToKParts(int32 N, int32 K)
{
    TArray<int32> Result;
    Result.Init(N / K, K);

    int32 Remaining = N % K;
    for (int32& Num : Result)
    {
        if (Remaining > 0)
        {
            Num += 1;
            Remaining -= 1;
        }
    }

    int32 Index1, Index2, Temp;
    for (int32 i = 0; i < K * 10; ++i)
    {
        Index1 = FMath::RandRange(0, K - 1);
        Index2 = FMath::RandRange(0, K - 1);
        Temp = Result[Index1];
        Result[Index1] = Result[Index2];
        Result[Index2] = Temp;
    }

    return Result;
}

