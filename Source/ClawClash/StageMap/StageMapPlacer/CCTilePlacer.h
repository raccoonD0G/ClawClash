// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCTilePlacer.generated.h"

UCLASS()
class CLAWCLASH_API UCCTilePlacer : public UObject
{
	GENERATED_BODY()
};
/*
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "PaperTileLayer.h"
#include "CCTilePlacer.generated.h"


USTRUCT()
struct FCCFieldTileSet
{
	GENERATED_BODY()
	public:
	FPaperTileInfo LeftTile;
	FPaperTileInfo MiddleTile;
	FPaperTileInfo RightTile;
};



class UPaperTileMapComponent;
class UPaperTileSet;

UCLASS()
class CLAWCLASH_API UCCTilePlacer : public UObject
{
	GENERATED_BODY()

// Layer Section
protected:
	const float FieldTileY = 50.1f;

// TileMap Info Section
protected:
	int32 TileMapWidth;
	int32 TileMapHeight;
	float TileWidth;
	float TileHeight;

	UPROPERTY()
	TMap<EFieldType, FCCFieldTileSet> TileSetPerFieldDic;

	UPROPERTY()
	TMap<ETileType, FPaperTileInfo> TileInfoPerTileDic;

public:
	UPROPERTY()
	TArray<TObjectPtr<class UCCTileCollider>> ColliderArr;

// TileMap Section
public:
	void InitializeTileMap(UPaperTileMapComponent* TileMapComponent, UPaperTileSet* TileSet, int32 Rows, int32 Columns, float NewTileWidth, float NewTileHeight);
	void InitializeTileSet(UPaperTileSet* NewFieldTileSet);
	void CreatAllField(UPaperTileMapComponent* TileMapComponent);
	void FillInBasic(UPaperTileMapComponent* TileMapComponent);
	void SetupTileColliders(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row);

protected:
	FVector GetWorldLocationFromTile(UPaperTileMapComponent* TileMapComponent, int32 Row, int32 Column);
	void SetTileIfPossible(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Layer, struct FPaperTileInfo TileInfo, bool bEmptyOnly = true);
	void CreateFieldByType(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 Length);
	bool CreatNoneBasicFieldTile(UPaperTileMapComponent* TileMapComponent, EFieldType CurrentType, int32 Column, int32 Row, int32 LengthOfField);
	void CreatHill(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length, int32 StairLength);
	void CreateBasic(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreateRaccoonHouse(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreateDogHouse(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreatWaterSide(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreateCave(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreateAsphalt(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
	void CreateEmpty(UPaperTileMapComponent* TileMapComponent, int32 Column, int32 Row, int32 Length);
};
*/