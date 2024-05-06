// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCBoxQuadTreeNode.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCBoxQuadTreeNode : public UObject
{
	GENERATED_BODY()

protected:
    UPROPERTY()
    TObjectPtr<UCCBoxQuadTreeNode> Children[4] = { nullptr, nullptr, nullptr, nullptr };

    FVector2D BoundsMin, BoundsMax;
    TArray<FBox2D> Objects;
    static const int MaxObjects = 4;
    static const int MaxDepth = 4;
    int Depth;

public:
    void Initialize(const FVector2D& InBoundsMin, const FVector2D& InBoundsMax, int InDepth);
    bool Insert(const FBox2D& Object);
    bool IsColliding(const FBox2D& Object) const;
protected:
    void Subdivide();
    int GetChildIndex(const FBox2D& Object) const;
};
