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
	if (bIsBottom == false) CreateNecessaryHill();
	DividePlatform();
}

void UCCFloor::CreateNecessaryHill()
{
	NecessaryHill = NewObject<UCCField>(this);
	FCCFieldInfo* HillInfo = UCCManagers::GetInstance()->GetStageMapManager()->FieldInfoMap.Find(EFieldType::HillField);
	int32 LengthOfHill = FMath::RandRange(HillInfo->MinLength, HillInfo->MaxLength);
	int32 StartPos = FMath::RandRange(0, Length - LengthOfHill);
	NecessaryHill->Init(StartPos, LengthOfHill, EFieldType::HillField);
}

void UCCFloor::DividePlatform()
{
	if (bIsBottom == false)
	{
		TObjectPtr<UCCPlatform> BeforeHill = NewObject<UCCPlatform>();
		BeforeHill->Init(0, NecessaryHill->GetStartPos() + 1, false);
		TObjectPtr<UCCPlatform> AfterHill = NewObject<UCCPlatform>();
		AfterHill->Init(NecessaryHill->GetStartPos() + NecessaryHill->GetLength() + 1, Length - NecessaryHill->GetStartPos() - NecessaryHill->GetLength(), false);
		PlatformArr.Add(BeforeHill);
		PlatformArr.Add(AfterHill);
	}
	else
	{
		TObjectPtr<UCCPlatform> SinglePlatform = NewObject<UCCPlatform>();
		SinglePlatform->Init(0, Length, true);
		PlatformArr.Add(SinglePlatform);
	}
}

int32 UCCFloor::GetNecessaryHillLength()
{
	return NecessaryHill->GetLength();
}

int32 UCCFloor::GetNecessaryHillStartPos()
{
	return NecessaryHill->GetStartPos();
}

bool UCCFloor::GetIsBottom()
{
	return bIsBottom;
}

