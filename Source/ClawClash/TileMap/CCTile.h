// CCTile.h
#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EGroundType : uint8
{
    None UMETA(DisplayName = "None"),
    Basic UMETA(DisplayName = "Basic"),
    Waterside UMETA(DisplayName = "Waterside"),
    Asphalt UMETA(DisplayName = "Asphalt"),
    Cave UMETA(DisplayName = "Cave"),
    Hill UMETA(DisplayName = "Hill"),
    HillSpace UMETA(DisplayName = "HillSpace"),
    Empty UMETA(DisplayName = "Empty")

};
