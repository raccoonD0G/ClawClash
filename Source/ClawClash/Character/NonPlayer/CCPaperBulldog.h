// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/NonPlayer/CCPaperNonPlayer.h"
#include "ClawClash/Interfaces/CCRushable.h"
#include "ClawClash/Interfaces/CCReadyRushable.h"
#include "CCPaperBulldog.generated.h"

UENUM(BlueprintType)
enum class EBulldogState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	ReadyRush UMETA(DisplayName = "ReadyRush"),
	Rush UMETA(DisplayName = "Rush"),
};

class UPlayerDetectorComponent;
class UDamageSphereComponent;

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperBulldog : public ACCPaperNonPlayer, public ICCRushable, public ICCReadyRushable
{
	GENERATED_BODY()

public:
	ACCPaperBulldog();

	virtual void BeginPlay() override;

// Sprite Section
public:
	virtual void FaceDirection(FVector Dir) override;

// Component Section
protected:
	TObjectPtr<UPlayerDetectorComponent> PlayerDetectorComponent;
	
// State Section
protected:
	EBulldogState CurrentState;

public:
	void SetCurrentState(EBulldogState NewState);
	FORCEINLINE EBulldogState GetCurrentState() { return CurrentState; }

// Animation Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> ReadyRushAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> RushAnimation;

// Stat Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RushMoveSpeed;

// Rush Section
protected:
	UPROPERTY()
	TObjectPtr<UDamageSphereComponent> DamageSphereComponent;

public:
	virtual void StartReadyRush() override;
	virtual float GetReadyRushTime() override;
	virtual void StartRush() override;
	virtual float GetRushTime() override;
	virtual void EndRush() override;

// Move Section
public :
	virtual void StartMove() override;
	virtual void EndMove() override;
};