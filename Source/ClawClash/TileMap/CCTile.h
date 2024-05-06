// CCTile.h
#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class ETileType : uint8
{
    Empty UMETA(DisplayName = "Empty"),
    Basic UMETA(DisplayName = "Basic"),
    WatersideLeft UMETA(DisplayName = "Waterside"),
    Waterside UMETA(DisplayName = "Waterside"),
    WatersideRight UMETA(DisplayName = "Waterside"),
    AsphaltLeft UMETA(DisplayName = "Asphalt"),
    Asphalt UMETA(DisplayName = "Asphalt"),
    AsphaltRight UMETA(DisplayName = "Asphalt"),
    CaveLeft UMETA(DisplayName = "Cave"),
    Cave UMETA(DisplayName = "Cave"),
    CaveRight UMETA(DisplayName = "Cave"),
    HillLeft UMETA(DisplayName = "Hill"),
    Hill UMETA(DisplayName = "Hill"),
    HillRight UMETA(DisplayName = "Hill"),
    HillSpace UMETA(DisplayName = "HillSpace"),
    
};

UENUM(BlueprintType)
enum class EFieldType : uint8
{
    EmptyField UMETA(DisplayName = "Empty"),
    BasicField UMETA(DisplayName = "Basic"),
    WatersideField UMETA(DisplayName = "Waterside"),
    AsphaltField UMETA(DisplayName = "Asphalt"),
    CaveField UMETA(DisplayName = "Cave"),
    HillField UMETA(DisplayName = "Hill"),
};

UENUM(BlueprintType)
enum class EWaterSideFeature : uint8
{
    Weed0Feature UMETA(DisplayName = "Weed"),
};

