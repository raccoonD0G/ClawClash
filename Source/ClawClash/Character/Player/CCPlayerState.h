// CCPlayerState.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Jump UMETA(DisplayName = "Jump"),
    Move UMETA(DisplayName = "Move"),
    Falling UMETA(DisplayName = "Falling"),
    Land UMETA(DisplayName = "Land")
};