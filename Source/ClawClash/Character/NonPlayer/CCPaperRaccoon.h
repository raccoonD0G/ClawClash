// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "CCPaperRaccoon.generated.h"

UENUM(BlueprintType)
enum class ERaccoonState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	ReadyAttack UMETA(DisplayName = "ReadyAttack"),
	Attack UMETA(DisplayName = "Attack"),
};

class UPlayerDetectorComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperRaccoon : public ACCPaperNonPlayer
{
	GENERATED_BODY()
	
public:
	ACCPaperRaccoon();

	virtual void BeginPlay() override;
	
// Component Section
protected:
	TObjectPtr<UPlayerDetectorComponent> PlayerDetectorComponent;

// State Section
protected:
	ERaccoonState CurrentState;

public:
	void SetCurrentState(ERaccoonState NewState);
	FORCEINLINE ERaccoonState GetCurrentState() { return CurrentState; }

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> ReadyAttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> AttackAnimation;

// Stat Section
protected:
	float MoveSpeed;
};
