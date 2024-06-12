// Fill out your copyright notice in the Description page of Project Settings.


#include "CCUnionFind.h"

void UCCUnionFind::Init(int32 Size)
{
    Parent.SetNum(Size);
    Rank.SetNum(Size);
    for (int32 i = 0; i < Size; ++i)
    {
        Parent[i] = i;
        Rank[i] = 0;
    }
}

int32 UCCUnionFind::Find(int32 Element)
{
    if (Parent[Element] != Element)
    {
        Parent[Element] = Find(Parent[Element]);
    }
    return Parent[Element];
}

void UCCUnionFind::Union(int32 Element1, int32 Element2)
{
    int32 Root1 = Find(Element1);
    int32 Root2 = Find(Element2);

    if (Root1 != Root2)
    {
        if (Rank[Root1] > Rank[Root2])
        {
            Parent[Root2] = Root1;
        }
        else if (Rank[Root1] < Rank[Root2])
        {
            Parent[Root1] = Root2;
        }
        else
        {
            Parent[Root2] = Root1;
            Rank[Root1]++;
        }
    }
}