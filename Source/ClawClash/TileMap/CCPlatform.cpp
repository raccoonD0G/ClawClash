// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlatform.h"
#include "CCField.h"

#include "ClawClash/Managers/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/TileMap/CCStageMap.h"

#include "Kismet/GameplayStatics.h"

#include "Math/UnrealMathUtility.h"

UCCPlatform::UCCPlatform()
{
	
	StartPos = 0;
	Length = 0;
}

EFieldType UCCPlatform::GetRandomField(const TMap<EFieldType, float>& FieldRatioMap)
{
    float TotalProbability = 0.0f;
    for (const TPair<EFieldType, float>& Pair : FieldRatioMap) {
        TotalProbability += Pair.Value;
    }

    float RandomValue = FMath::FRandRange(0.0f, TotalProbability);

    float CumulativeProbability = 0.0f;
    for (const TPair<EFieldType, float>& Pair : FieldRatioMap) {
        CumulativeProbability += Pair.Value;
        if (RandomValue <= CumulativeProbability) {
            return Pair.Key;
        }
    }

    return EFieldType::NoneField;
}

void UCCPlatform::Init(int32 NewStartPos, int32 NewLength, bool NewIsBottom)
{
	StartPos = NewStartPos;
	Length = NewLength;
    bIsBottom = NewIsBottom;
	CreatFieldOnPlatform();
}

void UCCPlatform::CreatFieldOnPlatform()
{
    const TMap<EFieldType, float>& FieldRatioMap = UCCManagers::GetInstance()->GetStageMapManager()->FieldRatioMap;
    int32 i;
	for (i = 0; i < Length; i++)
	{
        EFieldType FieldType;
        do
        {
            FieldType = GetRandomField(FieldRatioMap);
        } while (bIsBottom == true && FieldType == EFieldType::HillField);
        
        FCCFieldInfo* FieldInfo = UCCManagers::GetInstance()->GetStageMapManager()->FieldInfoMap.Find(FieldType);
        int32 FieldLength = FMath::RandRange(FieldInfo->MinLength, FieldInfo->MaxLength);
        if (i + FieldLength < Length)
        {
            UCCField* NewField = NewObject<UCCField>(this);
            NewField->Init(i, FieldLength, FieldType);
            FieldsInPlatformArr.Add(NewField);
            i += FieldLength;
        }
        else
        {
            break;
        }
	}
    TArray<int32> DecomposeNumArr = UCCManagers::GetInstance()->DecomposeNumberToKParts(Length - i, FieldsInPlatformArr.Num());
    for (int32 j = 0; j < FieldsInPlatformArr.Num(); j++)
    {
        for (int32 k = j; k < FieldsInPlatformArr.Num(); k++) FieldsInPlatformArr[k]->AddStartPos(DecomposeNumArr[j]);
    }
    return;
}

int32 UCCPlatform::GetStartPos()
{
    return StartPos;
}
