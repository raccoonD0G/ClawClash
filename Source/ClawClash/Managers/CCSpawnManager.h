// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/TileMap/CCStageMap.h"
#include "CCSpawn.h"
#include "CCSpawnManager.generated.h"

USTRUCT()
struct FSpawnableField
{
	GENERATED_BODY()
public:
	FVector LeftEnd;
	FVector RightEnd;
	ESpawnableType SpawnableType;
};

USTRUCT()
struct FSpawnableFieldArrContainer
{
	GENERATED_BODY()
public:
	TArray<FSpawnableField> SpawnableFieldArr;
};
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	UCCSpawnManager();

public:
	TMap<EFieldType, FSpawnableFieldArrContainer> SpawnFieldMap;
};
