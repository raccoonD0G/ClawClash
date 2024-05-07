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

// Feature
UENUM(BlueprintType)
enum class EBuildingFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    House0Feature UMETA(DisplayName = "House0Feature"),
    House1Feature UMETA(DisplayName = "House1Feature"),
    House2Feature UMETA(DisplayName = "House2Feature"),
    House3Feature UMETA(DisplayName = "House3Feature"),
    House4Feature UMETA(DisplayName = "House4Feature"),
    House5Feature UMETA(DisplayName = "House5Feature"),
    House6Feature UMETA(DisplayName = "House6Feature"),
};

UENUM(BlueprintType)
enum class ECarFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Car0Feature UMETA(DisplayName = "Car0"),
    Car1Feature UMETA(DisplayName = "Car1"),
};

UENUM(BlueprintType)
enum class EWeedFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Weed0Feature UMETA(DisplayName = "Weed0"),
    Weed1Feature UMETA(DisplayName = "Weed1"),
    Weed2Feature UMETA(DisplayName = "Weed2"),
    Weed3Feature UMETA(DisplayName = "Weed3")
};

UENUM(BlueprintType)
enum class EGrassFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Grass0Feature UMETA(DisplayName = "Grass0"),
    Grass1Feature UMETA(DisplayName = "Grass1"),
    Grass2Feature UMETA(DisplayName = "Grass2"),
    Grass3Feature UMETA(DisplayName = "Grass3"),
};

UENUM(BlueprintType)
enum class ELightFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    ShortLightFeature UMETA(DisplayName = "ShortLightFeature"),
    MiddleLightFeature UMETA(DisplayName = "MiddleLightFeature"),
    LongLightFeature UMETA(DisplayName = "LongLightFeature"),
};

UENUM(BlueprintType)
enum class ERockFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Stone0Feature UMETA(DisplayName = "Stone0Feature"),
    Stone1Feature UMETA(DisplayName = "Stone1Feature"),
    Stone2Feature UMETA(DisplayName = "Stone2Feature"),
    Stone3Feature UMETA(DisplayName = "Stone3Feature"),
    Stone4Feature UMETA(DisplayName = "Stone4Feature"),
};

// Legacy
UENUM(BlueprintType)
enum class EWaterSideFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Weed0Feature UMETA(DisplayName = "Weed0"),
    Weed1Feature UMETA(DisplayName = "Weed1")
};

UENUM(BlueprintType)
enum class EBasicFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Grass0Feature UMETA(DisplayName = "Grass0"),
    Grass1Feature UMETA(DisplayName = "Grass1"),
    Grass2Feature UMETA(DisplayName = "Grass2"),
    Grass3Feature UMETA(DisplayName = "Grass3"),
};

UENUM(BlueprintType)
enum class EAsphaltFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    ShortLightFeature UMETA(DisplayName = "ShortLightFeature"),
    LongLightFeature UMETA(DisplayName = "LongLightFeature"),
};

UENUM(BlueprintType)
enum class ECaveFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Stone0Feature UMETA(DisplayName = "Stone0Feature"),
    Stone1Feature UMETA(DisplayName = "Stone1Feature"),
    Stone2Feature UMETA(DisplayName = "Stone2Feature"),
    Stone3Feature UMETA(DisplayName = "Stone3Feature"),
    Stone4Feature UMETA(DisplayName = "Stone4Feature"),
};

