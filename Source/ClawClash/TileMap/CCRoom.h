// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCRoom.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCRoom : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 X;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Y;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Width;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Height;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UCCFloor> FloorInBox;

    int32 FloorStartX;
    int32 FloorStartY;

    void Init(int32 InX, int32 InY, int32 InWidth, int32 InHeight);

    FVector2D GetCenter() const;

    void GenerateFloor();
};
