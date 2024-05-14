// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/TileMap/CCStageMap.h"
#include "CCField.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCField : public UObject
{
	GENERATED_BODY()
	
public:
	UCCField();

protected:
	EFieldType FieldType;
	int32 StartPos;
	int32 Length;

public:
	int32 GetStartPos();
	void AddStartPos(int32 AddedNum);
	int32 GetLength();
	EFieldType GetFieldType();
	void SetStartPos(int32 NewStartPos);
	void Init(int32 NewStartPos, int32 NewLength, EFieldType NewFieldType);
};
