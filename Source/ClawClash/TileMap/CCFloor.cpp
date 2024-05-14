// Fill out your copyright notice in the Description page of Project Settings.


#include "CCFloor.h"

#include "ClawClash/Managers/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"
#include "CCField.h"
#include "CCPlatform.h"

#include "Kismet/GameplayStatics.h"
#include "ClawClash/CCGameInstance.h"

#include "Math/UnrealMathUtility.h"

void UCCFloor::Init(int32 FloorLength, bool bIsBottomOrNot)
{
	Length = FloorLength;
	bIsBottom = bIsBottomOrNot;
	DividePlatform();
}

void UCCFloor::DividePlatform()
{
	int32 LastFieldEnd = 0;
	for (int32 i = 0; i < EmptyField.Num(); i++)
	{
		TObjectPtr<UCCPlatform> NewPlatform = NewObject<UCCPlatform>(this);
		NewPlatform->Init(LastFieldEnd + 1, EmptyField[i]->GetStartPos() - LastFieldEnd, bIsBottom);
		PlatformArr.Add(NewPlatform);
		LastFieldEnd = EmptyField[i]->GetStartPos() + EmptyField[i]->GetLength();
	}
	TObjectPtr<UCCPlatform> NewPlatform = NewObject<UCCPlatform>(this);
	NewPlatform->Init(LastFieldEnd, Length - LastFieldEnd, bIsBottom);
	PlatformArr.Add(NewPlatform);
}

bool UCCFloor::GetIsBottom()
{
	return bIsBottom;
}

