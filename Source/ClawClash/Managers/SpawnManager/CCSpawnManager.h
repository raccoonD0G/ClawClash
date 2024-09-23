// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCSpawn.h"
#include "CCSpawnManager.generated.h"



class UPaperFlipbook;
class ACCSpawner;
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	UCCSpawnManager();
	static UCCSpawnManager* GetInstance();
	void Init();

protected:
	static UCCSpawnManager* Instance;

// Data Section
public:
	const TArray<UPaperFlipbook*>& GetRatSpriteArr() const;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UPaperFlipbook>> RatSpriteArr;
};
