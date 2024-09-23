// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "DamageSphereComponent.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API UDamageSphereComponent : public USphereComponent
{
	GENERATED_BODY()
public:
    UDamageSphereComponent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float DamageRadius;

public:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
