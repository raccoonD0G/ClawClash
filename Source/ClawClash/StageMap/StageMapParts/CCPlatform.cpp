// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPlatform.h"
#include "CCField.h"

#include "ClawClash/Managers/StageMapManager/CCStageMapManager.h"
#include "ClawClash/Managers/CCGameManager.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileMapParts.h"

#include "Kismet/GameplayStatics.h"

#include "Math/UnrealMathUtility.h"

#include "ClawClash/StageMap/CCTileMapActor.h"
#include "CCBasicField.h"
#include "CCWatersideField.h"
#include "CCAsphaltField.h"
#include "CCCaveField.h"
#include "CCHillField.h"
#include "CCRaccoonHouseField.h"
#include "CCDogHouseField.h"

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

    return EFieldType::BasicField;
}

void UCCPlatform::Init(ACCTileMapActor* NewOwningTileMap, FIntVector2 NewTileMapPos, int32 NewLength)
{
    UCCTileMapParts::Init(NewTileMapPos);
	Length = NewLength;
    OwningTileMap = NewOwningTileMap;
	CreatFieldOnPlatform();
}

void UCCPlatform::CreatFieldOnPlatform()
{
    const TMap<EFieldType, float>& FieldRatioMap = UCCStageMapManager::GetInstance()->FieldRatioMap;
    int32 TotalFieldLength = 0;

    while (true)
    {
        EFieldType FieldType;
        FieldType = GetRandomField(FieldRatioMap);

        FCCFieldInfo* FieldInfo = UCCStageMapManager::GetInstance()->FieldInfoMap.Find(FieldType);
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
            UCCField* NewField;

            switch (FieldType)
            {
            case EFieldType::BasicField:
                NewField = NewObject<UCCBasicField>();
                break;
            case EFieldType::WatersideField:
                NewField = NewObject<UCCWatersideField>();
                break;
            case EFieldType::AsphaltField:
                NewField = NewObject<UCCAsphaltField>();
                break;
            case EFieldType::CaveField:
                NewField = NewObject<UCCCaveField>();
                break;
            case EFieldType::HillField:
                NewField = NewObject<UCCHillField>();
                break;
            case EFieldType::RaccoonHouseField:
                NewField = NewObject<UCCRaccoonHouseField>();
                break;
            case EFieldType::DogHouseField:
                NewField = NewObject<UCCDogHouseField>();
                break;
            default:
                NewField = nullptr;
            }

            NewField->Init(OwningTileMap, FIntVector2(0, 0), FieldLength, FieldType);
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

    TArray<int32> DecomposeNumArr = UCCUtils::DecomposeNumberToKParts(Length - TotalFieldLength, FieldArr.Num() + 1);
    for (int32 j = 0; j < FieldArr.Num(); j++)
    {
        for (int32 k = j; k < FieldArr.Num(); k++) FieldArr[k]->AddStartPos(DecomposeNumArr[j]);
    }

    for (int32 j = 0; j < FieldArr.Num(); j++)
    {
        FIntVector2 Interval;
        if (j + 1 != FieldArr.Num())
        {
            Interval = FieldArr[j]->GetEndPos() - FieldArr[j + 1]->GetStartPos();
        }
        else
        {
            Interval = GetEndPos() - FieldArr[j]->GetEndPos();
        }
        
        if (Interval.X > 1)
        {
            UCCBasicField* BasicField = NewObject<UCCBasicField>();
            FIntVector2 StartPos = FieldArr[j]->GetEndPos();
            StartPos.X;
            BasicField->Init(OwningTileMap, StartPos, Interval.X, EFieldType::BasicField);
            FieldArr.Add(BasicField);
        }
    }

    if (FieldArr.Num() == 0)
    {
        UCCBasicField* BasicField = NewObject<UCCBasicField>();
        BasicField->Init(OwningTileMap, GetStartPos(), GetEndPos().X - GetStartPos().X, EFieldType::BasicField);
        FieldArr.Add(BasicField);
    }

    for (UCCField *Field : FieldArr)
    {
        Field->CreateTile();
        Field->CreateSprite();
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
