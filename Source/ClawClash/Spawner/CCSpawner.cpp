// Fill out your copyright notice in the Description page of Project Settings.


#include "CCSpawner.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"

void ACCSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACCSpawner::SpawnCharacter, SpawnInterval, true, SpawnInterval);
}

void ACCSpawner::Init(FSpawnableField SpawnableField)
{
	LeftEnd = SpawnableField.LeftEnd;
	RightEnd = SpawnableField.RightEnd;
	MaxCharacterNum = SpawnableField.MaxCharacterNum;
}

void ACCSpawner::SpawnCharacter()
{
	if (MaxCharacterNum > Charaters.Num())
	{
		float RandomX = FMath::FRandRange(LeftEnd.X, RightEnd.X);
		FVector SpawnPos = LeftEnd;
		SpawnPos.X = RandomX;

		FActorSpawnParameters SpawnParameters;
		ACCPaperNonPlayer* SpawnedCharacter = GetWorld()->SpawnActor<ACCPaperNonPlayer>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParameters);
		SpawnedCharacter->OnNonPlayerCharacterDestroyed.AddDynamic(this, &ACCSpawner::OnBeginDestroy);
		SpawnedCharacter->Init(LeftEnd.X, RightEnd.X);
		Charaters.Add(SpawnedCharacter);
	}

	if (Charaters.Num() >= MaxCharacterNum)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void ACCSpawner::OnBeginDestroy(ACCPaperNonPlayer* DestroyedCharacter)
{
	Charaters.Remove(DestroyedCharacter);

	if ((!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle)) && Charaters.Num() < MaxCharacterNum)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACCSpawner::SpawnCharacter, SpawnInterval, true, SpawnInterval);
	}
}
