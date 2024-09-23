// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, int32, NewCurrentHealth, int32, MaxHp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAWCLASH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void Init(int32 NewMaxHp);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// Health Section
protected:
	int32 CurrentHp;

	UPROPERTY(EditAnywhere, Category = "Hp")
	int32 MaxHp;

public:
	FHealthChangedDelegate OnHealthChanged;

	void GetDamaged(int32 Amount);
	void GetHeal(int32 Amount);
	FORCEINLINE float GetHealthRatio() { return CurrentHp / MaxHp; }
		
};
