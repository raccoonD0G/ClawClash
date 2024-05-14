// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCFloor.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCFloor : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(int32 FloorLength, bool bIsBottomOrNot);
	void DividePlatform();
	bool GetIsBottom();

	UPROPERTY()
	TArray<TObjectPtr<class UCCPlatform>> PlatformArr;
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<class UCCField>> EmptyField;

	bool bIsBottom;
	int32 Length;
};
