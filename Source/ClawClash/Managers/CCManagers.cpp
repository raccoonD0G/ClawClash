// Fill out your copyright notice in the Description page of Project Settings.


#include "CCManagers.h"
#include "ClawClash/Managers/CCStageMapManager.h"
#include "CCManagers.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "PaperSprite.h"

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

int32 UCCManagers::GetRandomIndexByProbability(const TArray<float>& Probabilities)
{
    if (Probabilities.Num() == 0)
    {
        return -1;
    }

    float TotalProbability = 0.f;
    for (float Probability : Probabilities)
    {
        TotalProbability += Probability;
    }

    float RandomNumber = FMath::FRandRange(0.0f, TotalProbability);

    float AccumulatedProbability = 0.f;
    for (int32 i = 0; i < Probabilities.Num(); ++i)
    {
        AccumulatedProbability += Probabilities[i];
        if (RandomNumber <= AccumulatedProbability)
        {
            return i;
        }
    }

    return Probabilities.Num() - 1;
}

int32 UCCManagers::GetRandomIndexByProbability(const TArray<FCCFeatureInfo>& Probabilities)
{
    if (Probabilities.Num() == 0)
    {
        return -1;
    }

    float TotalProbability = 0.f;
    for (FCCFeatureInfo Probability : Probabilities)
    {
        TotalProbability += Probability.FeatureRatio;
    }

    float RandomNumber = FMath::FRandRange(0.0f, TotalProbability);

    float AccumulatedProbability = 0.f;
    for (int32 i = 0; i < Probabilities.Num(); ++i)
    {
        AccumulatedProbability += Probabilities[i].FeatureRatio;
        if (RandomNumber <= AccumulatedProbability)
        {
            return i;
        }
    }

    return Probabilities.Num() - 1;
}

TArray<class UPaperSprite*> UCCManagers::GetAllSpritesFromFolder(const FString& SpritePath)
{
    TArray<UPaperSprite*> Sprites;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName(*SpritePath));
    Filter.ClassPaths.Add(UPaperSprite::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        TObjectPtr<UObject> AssetObject = AssetData.GetAsset();
        UPaperSprite* Sprite = Cast<UPaperSprite>(AssetObject);
        if (Sprite)
        {
            Sprites.Add(Sprite);
        }
    }

    return Sprites;
}

int32 UCCManagers::GetEnumLength(UEnum* TargetEnum)
{
    return TargetEnum->NumEnums() - 1;
}

TArray<int32> UCCManagers::DecomposeNumberToKParts(int32 N, int32 K)
{
    K++;
    TArray<int32> Result;

    Result.Init(0, K);
    for (int32& Num : Result)
    {
        Num = FMath::RandRange(0, N);
        N -= Num;
    }

    return Result;
}

