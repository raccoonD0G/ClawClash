// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClawClash/Managers/SpawnManager/CCSpawn.h"
#include "CCSpawnerSpawner.generated.h"

USTRUCT()
struct FSpawnableField
{
	GENERATED_BODY()

public:
	FVector LeftEnd;
	FVector RightEnd;
	ESpawnableType SpawnableType;
	int32 MaxCharacterNum;

	FSpawnableField()
	{
		LeftEnd = FVector::Zero();
		RightEnd = FVector::Zero();
		SpawnableType = ESpawnableType::Rat;
		MaxCharacterNum = 0;
	}

	FSpawnableField(FVector NewLeftEnd, FVector NewRightEnd, ESpawnableType NewSpawnableType, int32 NewMaxCharacterNum)
	{
		LeftEnd = NewLeftEnd;
		RightEnd = NewRightEnd;
		SpawnableType = NewSpawnableType;
		MaxCharacterNum = NewMaxCharacterNum;
	}

	bool operator==(const FSpawnableField& Other) const
	{
		return LeftEnd == Other.LeftEnd &&
			RightEnd == Other.RightEnd &&
			SpawnableType == Other.SpawnableType &&
			MaxCharacterNum == Other.MaxCharacterNum;
	}
};

FORCEINLINE uint32 GetTypeHash(const FSpawnableField& Field)
{
	uint32 Hash = HashCombine(GetTypeHash(Field.LeftEnd), GetTypeHash(Field.RightEnd));
	Hash = HashCombine(Hash, GetTypeHash(static_cast<uint8>(Field.SpawnableType)));
	return HashCombine(Hash, GetTypeHash(Field.MaxCharacterNum));
}

class UPaperFlipbook;
class ACCSpawner;

UCLASS()
class CLAWCLASH_API ACCSpawnerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACCSpawnerSpawner();

protected:
	UPROPERTY()
	TMap<FSpawnableField, TObjectPtr<ACCSpawner>> SpawnableFieldMap;

public:
	void AddSpawnableField(FSpawnableField SpawnableField);

// Spawner Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ACCSpawner> RatSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ACCSpawner> RaccoonSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ACCSpawner> BulldogSpawnerClass;

};
