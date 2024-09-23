// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::Init(int32 NewMaxHp)
{
	MaxHp = NewMaxHp;
	CurrentHp = MaxHp;
}

void UHealthComponent::GetDamaged(int32 Amount)
{
	CurrentHp -= Amount;
	if (CurrentHp <= 0)
	{
		GetOwner()->Destroy();
	}
}

void UHealthComponent::GetHeal(int32 Amount)
{
	CurrentHp += Amount;
	if (CurrentHp > MaxHp)
	{
		CurrentHp = MaxHp;
	}
}

