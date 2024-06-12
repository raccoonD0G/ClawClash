// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCStageMap.generated.h"

class UCCPlatform;
class UCCRoom;
class UCCStageMapManager;
/**
 * 
 */

USTRUCT()
struct FPlatformEdge
{
	GENERATED_BODY()
public:

	int32 PlatformIndex1;
	int32 PlatformIndex2;
	float Weight;
	FIntVector2 Pos1;
	FIntVector2 Pos2;

	FPlatformEdge()
	{

	}

	FPlatformEdge(int32 InPlatformIndex1, int32 InPlatformIndex2, float InWeight, FIntVector2 InPos1, FIntVector2 InPos2)
		: PlatformIndex1(InPlatformIndex1), PlatformIndex2(InPlatformIndex2), Weight(InWeight), Pos1(InPos1), Pos2(InPos2)
	{
	}

	bool operator<(const FPlatformEdge& Other) const
	{
		return Weight < Other.Weight;
	}
};

UCLASS()
class CLAWCLASH_API UCCStageMap : public UObject
{
	GENERATED_BODY()
	
public:
	const TArray<UCCPlatform*>& GetPlatformArr();
	void Init();

protected:
	UPROPERTY()
	TObjectPtr<UCCStageMapManager> StageMapManager;

	UPROPERTY()
	TArray<TObjectPtr<UCCPlatform>> PlatformArr;

	void SplitSpace(TArray<UCCRoom*>& OutRooms, UCCRoom* Space, int32 MinWidth, int32 MinHeight, int32 Depth);
	void GenerateRooms(TArray<UCCRoom*>& OutRooms, int32 MapWidth, int32 MapHeight, int32 MinWidth, int32 MinHeight);
	float CalculatePlatformDistance(const UCCPlatform& Platform0, const UCCPlatform& Platform1, FIntVector2& Pos1, FIntVector2& Pos2);
	void GenerateMST();
	void CreatePlatformsAlongEdge(const FPlatformEdge& Edge);
};
