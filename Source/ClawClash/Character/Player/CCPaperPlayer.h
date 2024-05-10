// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClawClash/Character/CCPaperCharacter.h"
#include "InputActionValue.h"
#include "CCPlayerState.h"
#include "CCPaperPlayer.generated.h"
/**
 * 
 */
UCLASS()
class CLAWCLASH_API ACCPaperPlayer : public ACCPaperCharacter
{
	GENERATED_BODY()
	
public:
	ACCPaperPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCurrentState(EPlayerState NewState);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void UpdateIdle();
	void UpdateMove();
	void UpdateJump();
	void UpdateLand();
	void UpdateFalling();

	float PlayerIdleThreshold = 0.1f;
	bool ShouldJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlayerState CurrentState;


// Material Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	TObjectPtr<class UMaterialInterface> DefaultSpriteMaterial;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
//Input Section
protected:

	float JumpStrength = 0.0f;
	float JumpHeight = 8000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	
	void Move(const FInputActionValue& Value);
	void StartJump();
	void Jump();
	void StopJumping();

//Flipbook Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> PlayerFlipbook;

//Animation Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> IdleAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> JumpAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> LandAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> MoveAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbook> FallingAnimation;
};
