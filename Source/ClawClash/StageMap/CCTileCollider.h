// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ClawClash/StageMap/CCStageMapDef.h"
#include "CCTileCollider.generated.h"
/**
 * 
 */
UCLASS()
class CLAWCLASH_API UCCTileCollider : public UBoxComponent
{
	GENERATED_BODY()
	
public:
    UCCTileCollider();
    void Init(EFieldType NewFieldType);
	
protected:
    EFieldType FieldType;
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    UPROPERTY()
    TObjectPtr<class ACCPaperPlayer> Player;

    float PlayerCapsuleHalfHeight;

};
