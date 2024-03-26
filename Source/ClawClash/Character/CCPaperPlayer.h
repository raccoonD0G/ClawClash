// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCPaperCharacter.h"
#include "InputActionValue.h"
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

protected:
	virtual void BeginPlay() override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
//Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	
	void Move(const FInputActionValue& Value);

//Flipbook Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> PlayerFlipbook;
};
