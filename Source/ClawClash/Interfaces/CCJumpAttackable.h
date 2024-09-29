// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCJumpAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCCJumpAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLAWCLASH_API ICCJumpAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FaceDirection(FVector Dir) = 0;
	virtual void StartJump() = 0;
	virtual float GetJumpTime() = 0;
	virtual void EndJump() = 0;
	virtual void StartAttack() = 0;
	virtual float GetAttackTime() = 0;
	virtual void EndAttack() = 0;
};
