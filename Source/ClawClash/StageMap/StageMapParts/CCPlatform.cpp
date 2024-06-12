// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlatform.h"
#include "CCField.h"

#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCManagers.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapParts.h"

#include "Kismet/GameplayStatics.h"

#include "Math/UnrealMathUtility.h"

UCCPlatform::UCCPlatform()
{
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

void UCCPlatform::Init(FIntVector2 NewTileMapPos, int32 NewLength, bool NewIsHillNecessary)
{
    UCCTileMapParts::Init(NewTileMapPos);
	Length = NewLength;
    bIsHillNecessary = NewIsHillNecessary;
	CreatFieldOnPlatform();
}

void UCCPlatform::CreatFieldOnPlatform()
{
    const TMap<EFieldType, float>& FieldRatioMap = UCCManagers::GetInstance()->GetStageMapManager()->FieldRatioMap;
    int32 TotalFieldLength = 0;

    if (bIsHillNecessary == true)
    {
        FCCFieldInfo* FieldInfo = UCCManagers::GetInstance()->GetStageMapManager()->FieldInfoMap.Find(EFieldType::HillField);
        int32 HillFieldLength = FMath::RandRange(FieldInfo->MinLength, FieldInfo->MaxLength);

        UCCField* NewField = NewObject<UCCField>(this);
        NewField->Init(FIntVector2(0, 0), HillFieldLength, EFieldType::HillField);
        FieldArr.Add(NewField);
        TotalFieldLength += HillFieldLength;
    }

    while (true)
    {
        EFieldType FieldType;
        FieldType = GetRandomField(FieldRatioMap);

        FCCFieldInfo* FieldInfo = UCCManagers::GetInstance()->GetStageMapManager()->FieldInfoMap.Find(FieldType);
        if (FieldInfo == nullptr)
        {
            UE_LOG(LogTemp, Log, TEXT("FieldInfo NULL"));
        }

        int32 FieldLength = FMath::RandRange(FieldInfo->MinLength, FieldInfo->MaxLength);
        if (TotalFieldLength + FieldLength > Length && TotalFieldLength + FieldInfo->MinLength <= Length)
        {
            FieldLength = FMath::RandRange(FieldInfo->MinLength, Length - TotalFieldLength);
        }

        if (TotalFieldLength + FieldLength <= Length)
        {
            UCCField* NewField = NewObject<UCCField>(this);
            NewField->Init(FIntVector2(0, 0), FieldLength, FieldType);
            FieldArr.Add(NewField);
            TotalFieldLength += FieldLength;
        }
        else break;
    }

    ShuffleArray(FieldArr);

    FIntVector2 NewFieldPos = TileMapPos;
    for (int32 i = 0; i < FieldArr.Num(); i++)
    {
        FieldArr[i]->SetTileMapPos(NewFieldPos);
        NewFieldPos.X += FieldArr[i]->GetLength();
    }

    TArray<int32> DecomposeNumArr = UCCManagers::GetInstance()->DecomposeNumberToKParts(Length - TotalFieldLength, FieldArr.Num() + 1);
    for (int32 j = 0; j < FieldArr.Num(); j++)
    {
        for (int32 k = j; k < FieldArr.Num(); k++) FieldArr[k]->AddStartPos(DecomposeNumArr[j]);
    }
}

const TArray<UCCField*>& UCCPlatform::GetFieldArr() const
{
    return FieldArr;
}

void UCCPlatform::ShuffleArray(TArray<TObjectPtr<UCCField>>& Array)
{
    for (int32 Index = Array.Num() - 1; Index > 0; Index--)
    {
        int32 SwapIndex = FMath::RandRange(0, Index);
        Array.Swap(Index, SwapIndex);
    }
}
