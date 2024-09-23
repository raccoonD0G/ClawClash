// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Components/HpBarComponent.h"
#include "ClawClash/Components/HealthComponent.h"

// Sets default values for this component's properties
UHpBarComponent::UHpBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHpBarComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	
	if (HealthComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UHpBarComponent need UHealthComponent"));
		return;
	}
	HealthComponent->OnHealthChanged.AddDynamic(this, &UHpBarComponent::UpdateHpBar);
}

void UHpBarComponent::UpdateHpBar(int32 NewCurrentHealth, int32 MaxHp)
{

}

