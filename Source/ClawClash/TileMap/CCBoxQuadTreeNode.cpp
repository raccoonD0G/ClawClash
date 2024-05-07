// Fill out your copyright notice in the Description page of Project Settings.


#include "CCBoxQuadTreeNode.h"

void UCCBoxQuadTreeNode::Initialize(const FVector2D& InBoundsMin, const FVector2D& InBoundsMax, int InDepth)
{
    BoundsMin = InBoundsMin;
    BoundsMax = InBoundsMax;
    Depth = InDepth;
}

bool UCCBoxQuadTreeNode::Insert(const FBox2D& Object)
{
    if (!FBox2D(BoundsMin, BoundsMax).Intersect(Object))
    {
        return false;
    }
    else
    {
        if (Objects.Num() < MaxObjects || Depth >= MaxDepth)
        {
            Objects.Add(Object);
            return true;
        }
        else
        {
            if (Children[0] == nullptr)
                Subdivide();

            int Index = GetChildIndex(Object);
            if (Index != -1)
            {
                return Children[Index]->Insert(Object);
            }
            else
            {
                Objects.Add(Object);
                return true;
            }
        }
    }
}

bool UCCBoxQuadTreeNode::IsColliding(const FBox2D& Object) const
{
    FBox2D BoundBox(BoundsMin, BoundsMax);

    if (!BoundBox.Intersect(Object))
        return false;

    for (const FBox2D& BoxInsideNode : Objects)
    {
        if (BoxInsideNode.Intersect(Object))
            return true;
    }

    if (Children[0] != nullptr)
    {
        for (const TObjectPtr<UCCBoxQuadTreeNode> Child : Children)
        {
            if (Child && Child->IsColliding(Object))
                return true;
        }
    }

    return false;
}

void UCCBoxQuadTreeNode::Subdivide()
{
    FVector2D Mid = BoundsMin + (BoundsMax - BoundsMin) / 2.0f;

    Children[0] = NewObject<UCCBoxQuadTreeNode>();
    Children[0]->Initialize(BoundsMin, Mid, Depth + 1);

    Children[1] = NewObject<UCCBoxQuadTreeNode>();
    Children[1]->Initialize(FVector2D(Mid.X, BoundsMin.Y), FVector2D(BoundsMax.X, Mid.Y), Depth + 1);

    Children[2] = NewObject<UCCBoxQuadTreeNode>();
    Children[2]->Initialize(FVector2D(BoundsMin.X, Mid.Y), FVector2D(Mid.X, BoundsMax.Y), Depth + 1);

    Children[3] = NewObject<UCCBoxQuadTreeNode>();
    Children[3]->Initialize(Mid, BoundsMax, Depth + 1);
}

int UCCBoxQuadTreeNode::GetChildIndex(const FBox2D& Object) const
{
    FVector2D Mid = BoundsMin + (BoundsMax - BoundsMin) / 2.0f;
    bool Top = (Object.Min.Y < Mid.Y && Object.Max.Y <= Mid.Y);
    bool Bottom = (Object.Min.Y >= Mid.Y);

    if (Object.Min.X < Mid.X && Object.Max.X <= Mid.X)
    {
        if (Top) return 0;
        if (Bottom) return 2;
    }
    else if (Object.Min.X >= Mid.X)
    {
        if (Top) return 1;
        if (Bottom) return 3;
    }

    return -1;
}