// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/TileMap/CCStageMap.h"
#include "CCPlatform.generated.h"
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCPlatform : public UObject
{
	GENERATED_BODY()
	
public:
	UCCPlatform();

protected:
	int32 StartPos;
	int32 Length;
	bool bIsBottom;

	EFieldType GetRandomField(const TMap<EFieldType, float>& FieldRatioMap);

public:
	void Init(int32 NewStartPos, int32 NewLength, bool NewIsBottom);
	void CreatFieldOnPlatform();
	int32 GetStartPos();
	int32 GetLength();

	UPROPERTY()
	TArray<TObjectPtr<class UCCField>> FieldsInPlatformArr;

	void ShuffleArray(TArray<TObjectPtr<UCCField>>& Array);
};
