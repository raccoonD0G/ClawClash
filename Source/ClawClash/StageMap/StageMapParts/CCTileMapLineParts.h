// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCTileMapParts.h"
#include "CCTileMapLineParts.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCTileMapLineParts : public UCCTileMapParts
{
	GENERATED_BODY()
	
protected:
	int32 Length;

public:
	FIntVector2 GetStartPos() const;
	int32 GetLength() const;
	FIntVector2 GetEndPos() const;
	
};
