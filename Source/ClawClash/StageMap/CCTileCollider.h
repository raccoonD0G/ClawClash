// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
	
protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    //UFUNCTION()
    //void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
    //AActor* NearbyActor;
    //FVector ActorShownPos;

    UPROPERTY()
    TObjectPtr<class ACCPaperPlayer> Player;

    float PlayerCapsuleHalfHeight;

};
