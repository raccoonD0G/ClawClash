// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerDetectorComponent.generated.h"

class USphereComponent;
class ACCPaperPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDetectedDelegate, ACCPaperPlayer*, DetectedPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerNotDetectedDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLAWCLASH_API UPlayerDetectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerDetectorComponent();

protected:
    virtual void BeginPlay() override;

public:
    FPlayerDetectedDelegate OnPlayerFirstDetected;
    FPlayerNotDetectedDelegate OnNoPlayerDetected;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
    USphereComponent* DetectionSphere;

    UPROPERTY(BlueprintReadOnly, Category = "Detection")
    TSet<TObjectPtr<ACCPaperPlayer>> DetectedPlayers;

    bool IsPlayerNearby();

    UFUNCTION()
    void OnPlayerDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlayerLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};