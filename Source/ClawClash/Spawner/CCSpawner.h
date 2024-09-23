// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClawClash/Spawner/CCSpawnerSpawner.h"
#include "CCSpawner.generated.h"

class ACCPaperNonPlayer;

UCLASS()
class CLAWCLASH_API ACCSpawner : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	FVector LeftEnd;
	FVector RightEnd;
	int32 MaxCharacterNum;
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<ACCPaperNonPlayer> SpawnClass;

	UPROPERTY()
	TSet<TWeakObjectPtr<ACCPaperNonPlayer>> Charaters;

	FTimerHandle TimerHandle;

public:
	void Init(FSpawnableField SpawnableField);
	virtual void SpawnCharacter();

	UFUNCTION()
	void OnBeginDestroy(ACCPaperNonPlayer* DestroyedCharacter);
};
