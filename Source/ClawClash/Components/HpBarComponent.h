// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpBarComponent.generated.h"

class UHealthComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASH_API UHpBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHpBarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// Health Section
protected:
	UPROPERTY()
	TWeakObjectPtr<UHealthComponent> HealthComponent;

	void UpdateHpBar(int32 NewCurrentHealth, int32 MaxHp);
};
