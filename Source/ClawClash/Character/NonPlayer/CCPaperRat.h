// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCPaperNonPlayer.h"
#include "CCPaperRat.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperRat : public ACCPaperNonPlayer
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
