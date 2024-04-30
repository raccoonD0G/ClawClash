// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"
#include "CCTileMapActor.generated.h"

UCLASS()
class CLAWCLASH_API ACCTileMapActor : public AActor
{
	GENERATED_BODY()
	
public:
    ACCTileMapActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

// TileMapSection
protected:

    UFUNCTION()
    int32 GetRandomIndexByProbability(const TArray<float>& Probabilities);

    UFUNCTION()
    void CreateGroundByType(EGroundType CurrentType, int32 Column, int32 Row);

    UFUNCTION()
    void CreatHill(int32 Column, int32 Row, int32 StairLength);

    UFUNCTION()
    void SetTileIfPossible(int32 Column, int32 Row, int32 Layer, FPaperTileInfo TileToSet);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPaperTileMapComponent> TileMapComponent;

    // Reference to the TileSet
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileMap")
    TObjectPtr<class UPaperTileSet> TileSet;

    UPROPERTY()
    TArray<FPaperTileInfo> GroundTileArr;

    UPROPERTY()
    TArray<float> GroundRatio;

    int32 TileMapWeidth = 512;
    int32 TileMapHeight = 128;
    int32 NumOfFloor = 4;
};

