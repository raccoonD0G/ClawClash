// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/CCPaperCharacter.h"
#include "CCPaperNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperNonPlayer : public ACCPaperCharacter
{
	GENERATED_BODY()
	
public:
	ACCPaperNonPlayer();
	virtual void BeginPlay() override;

// Sprite Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UPaperFlipbookComponent> PlayerFlipbook;

// Target Section
protected:
	float MaxLeftXPos;
	float MaxRightXPos;
	FVector TargetLocation;

public:
	float GetMaxLeftXPos() const;
	float GetMaxRightXPos() const;
	void SetMaxLeftXPos(float NewMaxLeftX);
	void SetMaxRightXPos(float NewMaxRightX);
	FVector GetTargetLocation() const;
	void SetTargetLocation(FVector NewTargetLocation);
	
};
