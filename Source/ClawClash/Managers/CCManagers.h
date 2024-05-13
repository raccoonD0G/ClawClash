// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCManagers.generated.h"


class UCCStageMapManager;
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

public:
    static UCCManagers* GetInstance();
    void SetInstanceNull();
	UCCStageMapManager* GetStageMapManager();

// Util Section
public:
    int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);
    int32 GetRandomIndexByProbability(const TArray<struct FCCFeatureInfo>& Probabilities);

    UFUNCTION()
    TArray<class UPaperSprite*> GetAllSpritesFromFolder(const FString& SpritePath);

    UFUNCTION()
    int32 GetEnumLength(UEnum* TargetEnum);

    UFUNCTION()
    TArray<int32> DecomposeNumberToKParts(int32 n, int32 k);
	
};
