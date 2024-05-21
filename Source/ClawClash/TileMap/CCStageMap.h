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
    None UMETA(DisplayName = "None"),
    
};

UENUM(BlueprintType)
enum class EFieldType : uint8
{
    NoneField  UMETA(DisplayName = "NoneField"),
    WatersideField UMETA(DisplayName = "Waterside"),
    AsphaltField UMETA(DisplayName = "Asphalt"),
    CaveField UMETA(DisplayName = "Cave"),
    HillField UMETA(DisplayName = "Hill"),
    RaccoonHouseField UMETA(DisplayName = "RaccoonHouse"),
    DogHouseField UMETA(DisplayName = "DogHouse")
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
enum class EFeatureType : uint8
{
    BuildingFeature UMETA(DisplayName = "BuildingFeature"),
    CarFeature UMETA(DisplayName = "CarFeature"),
    WeedFeature UMETA(DisplayName = "WeedFeature"),
    LightFeature UMETA(DisplayName = "LightFeature"),
    RockFeature UMETA(DisplayName = "RockFeature"),
    StoneFeature UMETA(DisplayName = "StoneFeature"),
    PlantFeature UMETA(DisplayName = "PlantFeature"),
    GrassFeature UMETA(DisplayName = "GrassFeature"),
    WaterFeature UMETA(DisplayName = "WaterFeature"),
    RatCaveFeature UMETA(DisplayName = "RatCaveFeature"),
    RacconHouseFeature UMETA(DisplayName = "RacconHouseFeature"),
    DogHouseFeature UMETA(DisplayName = "RacconHouseFeature"),
    TreeFeature UMETA(DisplayName = "TreeFeature"),
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
    Rock0Feature UMETA(DisplayName = "Rock0Feature"),
    Rock1Feature UMETA(DisplayName = "Rock1Feature"),
    Rock2Feature UMETA(DisplayName = "Rock2Feature"),
    Rock3Feature UMETA(DisplayName = "Rock3Feature"),
    Rock4Feature UMETA(DisplayName = "Rock4Feature"),
};

UENUM(BlueprintType)
enum class EStoneFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Stone0Feature UMETA(DisplayName = "Stone0Feature"),
    Stone1Feature UMETA(DisplayName = "Stone1Feature"),
    Stone2Feature UMETA(DisplayName = "Stone2Feature"),
    Stone3Feature UMETA(DisplayName = "Stone3Feature"),
    Stone4Feature UMETA(DisplayName = "Stone4Feature"),
};

UENUM(BlueprintType)
enum class EPlantFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Plant0Feature UMETA(DisplayName = "Plant0Feature"),
    Plant1Feature UMETA(DisplayName = "Plant1Feature"),
    Plant2Feature UMETA(DisplayName = "Plant2Feature"),
    Plant3Feature UMETA(DisplayName = "Plant3Feature"),
    Plant4Feature UMETA(DisplayName = "Plant4Feature"),
    Plant5Feature UMETA(DisplayName = "Plant5Feature"),
    Plant6Feature UMETA(DisplayName = "Plant6Feature"),
};

UENUM(BlueprintType)
enum class EGrassFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Grass0Feature UMETA(DisplayName = "Grass0Feature"),
    Grass1Feature UMETA(DisplayName = "Grass1Feature"),
    Grass2Feature UMETA(DisplayName = "Grass2Feature"),
    Grass3Feature UMETA(DisplayName = "Grass3Feature"),
};

UENUM(BlueprintType)
enum class EWaterFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Water0Feature UMETA(DisplayName = "Water0Feature"),

};

UENUM(BlueprintType)
enum class ERatCaveFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    RatCave0Feature UMETA(DisplayName = "RatCave0Feature"),
    RatCave1Feature UMETA(DisplayName = "RatCave1Feature"),
    RatCave2Feature UMETA(DisplayName = "RatCave2Feature"),
    RatCave3Feature UMETA(DisplayName = "RatCave3Feature"),
    RatCave4Feature UMETA(DisplayName = "RatCave4Feature"),
    RatCave5Feature UMETA(DisplayName = "RatCave5Feature"),
};

UENUM(BlueprintType)
enum class ERaccooHouseFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Raccoon0HouseFeature UMETA(DisplayName = "Raccoon0House"),
    Raccoon1HouseFeature UMETA(DisplayName = "Raccoon1House"),
    Raccoon2HouseFeature UMETA(DisplayName = "Raccoon2House"),
    Raccoon3HouseFeature UMETA(DisplayName = "Raccoon3House"),
    Raccoon4HouseFeature UMETA(DisplayName = "Raccoon4House"),
    Raccoon5HouseFeature UMETA(DisplayName = "Raccoon5House"),
};

UENUM(BlueprintType)
enum class EDogHouseFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    DogHouse0Feature UMETA(DisplayName = "EDogHouse0Feature"),
    DogHouse1Feature UMETA(DisplayName = "EDogHouse1Feature"),
    DogHouse2Feature UMETA(DisplayName = "EDogHouse2Feature"),
    DogHouse3Feature UMETA(DisplayName = "EDogHouse3Feature"),
};

UENUM(BlueprintType)
enum class ETreeFeature : uint8
{
    NoneFeature UMETA(DisplayName = "None"),
    Tree0Feature UMETA(DisplayName = "Tree0Feature")
};