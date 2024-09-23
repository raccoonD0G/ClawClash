// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCPaperNonPlayer.h"
#include "ClawClash/Interfaces/CCMoveable.h"
#include "CCPaperRat.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperRat : public ACCPaperNonPlayer
{
	GENERATED_BODY()
public:
	ACCPaperRat();

protected:
	virtual void BeginPlay() override;

// Move Section
public:
	virtual void StartMove() override;
	virtual void EndMove() override;
};
