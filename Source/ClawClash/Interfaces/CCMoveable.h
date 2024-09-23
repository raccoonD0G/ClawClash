// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCMoveable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCMoveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLAWCLASH_API ICCMoveable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FaceDirection(FVector Dir) = 0;
	virtual void StartMove() = 0;
	virtual void EndMove() = 0;
};
