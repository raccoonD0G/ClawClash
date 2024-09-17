// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawner.h"

#include "ClawClash/StageMap/CCStageMapDef.h"
#include "ClawClash/Character/NonPlayer/CCPaperRat.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/Managers/SpawnManager/CCSpawnManager.h"

// Sets default values
ACCSpawner::ACCSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACCSpawner::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACCSpawner::SpawnRat, 3.0f, true, 3.0f);
	
}

// Called every frame
void ACCSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACCSpawner::SpawnRat()
{
	for (FSpawnableField Field : UCCSpawnManager::GetInstance()->SpawnFieldMap.Find(EFieldType::CaveField)->SpawnableFieldArr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACCPaperRat* NewRat = GetWorld()->SpawnActor<ACCPaperRat>(RatClass, FVector(FMath::RandRange(Field.LeftEnd.X, Field.RightEnd.X), 0, Field.LeftEnd.Z), FRotator::ZeroRotator, SpawnParams);
		NewRat->SetMaxLeftXPos(Field.LeftEnd.X);
		NewRat->SetMaxRightXPos(Field.RightEnd.X);
	}
}

