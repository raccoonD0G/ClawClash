// GameDefinitions.h
#pragma once

#include "CoreMinimal.h"




class CLAWCLASH_API FGameDefinitions
{
public:
    UENUM(BlueprintType)
        enum class EPlayerState : uint8
    {
        Idle UMETA(DisplayName = "Idle"),
        Jump UMETA(DisplayName = "Jump"),
        Move UMETA(DisplayName = "Move")
    };
};