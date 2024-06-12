// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCUnionFind.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCUnionFind : public UObject
{
	GENERATED_BODY()
public:
    void Init(int32 Size);

    int32 Find(int32 Element);

    void Union(int32 Element1, int32 Element2);
    
protected:
    TArray<int32> Parent;
    TArray<int32> Rank;
};
