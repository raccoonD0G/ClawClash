// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCRushable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCRushable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLAWCLASH_API ICCRushable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FaceDirection(FVector Dir) = 0;
	virtual void StartRush() = 0;
	virtual bool IsRushEnd() = 0;
	virtual void EndRush() = 0;
};
