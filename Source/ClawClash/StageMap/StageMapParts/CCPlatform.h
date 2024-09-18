// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapLineParts.h"
#include "CCPlatform.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCPlatform : public UCCTileMapLineParts
{
	GENERATED_BODY()
	
public:
	UCCPlatform();

protected:

	EFieldType GetRandomField(const TMap<EFieldType, float>& FieldRatioMap);

	UPROPERTY()
	TArray<TObjectPtr<class UCCField>> FieldArr;
public:
	void Init(ACCTileMapActor* NewOwningTileMap, FIntVector2 NewTileMapPos, int32 NewLength);
	void CreatFieldOnPlatform();

	const TArray<class UCCField*>& GetFieldArr() const;

	void ShuffleArray(TArray<TObjectPtr<UCCField>>& Array);
	void FillInBasic();
};
