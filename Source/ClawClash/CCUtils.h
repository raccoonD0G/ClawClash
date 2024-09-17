// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"

#include "CCUtils.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCUtils : public UObject
{
	GENERATED_BODY()
	
public:
	template <typename T>
	static TArray<T*> GetAllResourceFromFolder(const FString& Path);

    UFUNCTION()
    static int32 GetEnumLength(UEnum* TargetEnum);

    UFUNCTION()
    static TArray<int32> DecomposeNumberToKParts(int32 n, int32 k);

    static int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);
};

template <typename T>
TArray<T*> UCCUtils::GetAllResourceFromFolder(const FString& Path)
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