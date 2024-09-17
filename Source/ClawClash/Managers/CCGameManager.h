// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCGameManager.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCGameManager : public UObject
{
	GENERATED_BODY()
	
// GameManager Section
protected:
    static UCCGameManager* Instance;

public:
    static UCCGameManager* GetInstance();
};

