// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/CCPaperCharacter.h"
#include "CCPaperNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperNonPlayer : public ACCPaperCharacter
{
	GENERATED_BODY()
	
public:
	ACCPaperNonPlayer();
	virtual void BeginPlay() override;

protected:
	float MaxLeftXPos;
	float MaxRightXPos;
public:
	float GetMaxLeftXPos();
	float GetMaxRightXPos();
	void SetMaxLeftXPos(float NewMaxLeftX);
	void SetMaxRightXPos(float NewMaxRightX);
	
};
