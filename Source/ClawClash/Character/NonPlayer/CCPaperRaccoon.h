// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "ClawClash/Interfaces/CCJumpAttackable.h"
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
class UDamageSphereComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperRaccoon : public ACCPaperNonPlayer, public ICCJumpAttackable
{
	GENERATED_BODY()
	
public:
	ACCPaperRaccoon();

	virtual void BeginPlay() override;

// Sprite Section
public:
	virtual void FaceDirection(FVector Dir) override;
	
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

// JumpAttack Section
protected:
	UPROPERTY()
	TObjectPtr<UDamageSphereComponent> DamageSphereComponent;
public:
	virtual void StartJump() override;
	virtual float GetJumpTime() override;
	virtual void EndJump() override;
	virtual void StartAttack() override;
	virtual float GetAttackTime() override;
	virtual void EndAttack() override;

// Move Section
public:
	virtual void StartMove() override;
	virtual void EndMove() override;
};
