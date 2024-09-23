// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/CCPaperCharacter.h"
#include "ClawClash/Interfaces/CCMoveable.h"
#include "CCPaperNonPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNonPlayerCharacterDestroyed, ACCPaperNonPlayer*, DestroyedCharacter);

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperNonPlayer : public ACCPaperCharacter, public ICCMoveable
{
	GENERATED_BODY()
	
public:
	ACCPaperNonPlayer();
	FOnNonPlayerCharacterDestroyed OnNonPlayerCharacterDestroyed;

protected:
	virtual void BeginDestroy() override;

// Sprite Section
protected:
	bool IsRightStart;

public:
	virtual void FaceDirection(FVector Dir) override;

// Init Section
public:
	virtual void Init(float NewMaxLeftXPos, float NewMaxRightX);

// Target Section
protected:
	float MaxLeftXPos;
	float MaxRightXPos;
	FVector TargetLocation;

public:
	FORCEINLINE float GetMaxLeftXPos() const { return MaxLeftXPos; }
	FORCEINLINE float GetMaxRightXPos() const { return MaxRightXPos; }
	FORCEINLINE void SetMaxLeftXPos(float NewMaxLeftX) { MaxLeftXPos = NewMaxLeftX; }
	FORCEINLINE void SetMaxRightXPos(float NewMaxRightX) { MaxRightXPos = NewMaxRightX; }
	FORCEINLINE FVector GetDest() const { return TargetLocation; }
	FORCEINLINE void SetDest(FVector NewTargetLocation) { TargetLocation = NewTargetLocation; }

// Move Section
public:
	virtual void StartMove() override;
	virtual void EndMove() override;
	
};
