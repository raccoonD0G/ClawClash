// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"

#include "CCManagers.generated.h"


class UCCStageMapManager;
class UCCSpawnManager;
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCManagers : public UObject
{
	GENERATED_BODY()
	
// Managers Section
protected:

    static TObjectPtr<UCCManagers> Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
	TObjectPtr<UCCStageMapManager> StageMapManager;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Managers")
    TObjectPtr<UCCSpawnManager> SpawnManager;

public:
    static UCCManagers* GetInstance();
    void SetInstanceNull();
	UCCStageMapManager* GetStageMapManager();
    UCCSpawnManager* GetSpawnManager();

// Util Section
public:
    int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);
    int32 GetRandomIndexByProbability(const TArray<struct FCCFeatureInfo>& Probabilities);

    template <typename T>
    TArray<T*> GetAllResourceFromFolder(const FString& Path);

    UFUNCTION()
    int32 GetEnumLength(UEnum* TargetEnum);

    UFUNCTION()
    TArray<int32> DecomposeNumberToKParts(int32 n, int32 k);
	
};

template <typename T>
TArray<T*> UCCManagers::GetAllResourceFromFolder(const FString& Path)
{
    TArray<T*> Resources;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    FARFilter Filter;
    Filter.PackagePaths.Add(FName(*Path));
    Filter.ClassPaths.Add(T::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        TObjectPtr<UObject> AssetObject = AssetData.GetAsset();
        T* Resource = Cast<T>(AssetObject);
        if (Resource)
        {
            Resources.Add(Resource);
        }
    }

    return Resources;
}
