// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/StageMap/StageMapParts/CCField.h"
#include "CCAsphaltField.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCAsphaltField : public UCCField
{
	GENERATED_BODY()
	
public:
	virtual void CreateTile() override;
	virtual void CreateSprite() override;
};
