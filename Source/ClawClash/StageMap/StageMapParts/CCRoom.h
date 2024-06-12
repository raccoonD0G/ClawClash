// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCTileMapParts.h"
#include "CCRoom.generated.h"

class UCCPlatform;
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCRoom : public UCCTileMapParts
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UCCPlatform> Platform;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Width;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Height;

    const UCCPlatform* GetPlatform() const;

    void Init(FIntVector2 NewTileMapPos, int32 InWidth, int32 InHeight);

    FIntVector2 GetCenter() const;

    UCCPlatform* GeneratePlatform();
};
