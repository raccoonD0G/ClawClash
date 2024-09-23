// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Spawner/CCSpawnerSpawner.h"
#include "ClawClash/Spawner/CCSpawner.h"

// Sets default values
ACCSpawnerSpawner::ACCSpawnerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ACCSpawnerSpawner::AddSpawnableField(FSpawnableField SpawnableField)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FVector SpawnPos = (SpawnableField.LeftEnd + SpawnableField.RightEnd) / 2.0f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACCSpawner* SpawnedSpawner;

	switch (SpawnableField.SpawnableType)
	{
	case ESpawnableType::Rat:
		if (!RatSpawnerClass)
		{
			return;
		}
		SpawnedSpawner = World->SpawnActor<ACCSpawner>(RatSpawnerClass, SpawnPos, FRotator::ZeroRotator, SpawnParams);
		break;
	case ESpawnableType::Raccoon:
		if (!RaccoonSpawnerClass)
		{
			return;
		}
		SpawnedSpawner = World->SpawnActor<ACCSpawner>(RaccoonSpawnerClass, SpawnPos, FRotator::ZeroRotator, SpawnParams);
		break;
	case ESpawnableType::Dog:
		if (!BulldogSpawnerClass)
		{
			return;
		}
		SpawnedSpawner = World->SpawnActor<ACCSpawner>(BulldogSpawnerClass, SpawnPos, FRotator::ZeroRotator, SpawnParams);
		break;
	default:
		SpawnedSpawner = nullptr;
		break;
	}

	SpawnedSpawner->Init(SpawnableField);
	SpawnableFieldMap.Add(SpawnableField, SpawnedSpawner);
}
