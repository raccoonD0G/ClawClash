// Fill out your copyright notice in the Description page of Project Settings.


#include "ClawClash/Components/PlayerDetectAIComponent.h"
#include "ClawClash/Character/Player/CCPaperPlayer.h"
#include "ClawClash/Components/PlayerDetectorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Sets default values for this component's properties
UPlayerDetectAIComponent::UPlayerDetectAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPlayerDetectAIComponent::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BlackboardComponent = GetOwner()->FindComponentByClass<UBlackboardComponent>();

	UPlayerDetectorComponent* PlayerDetector = InPawn->FindComponentByClass<UPlayerDetectorComponent>();
	if (PlayerDetector == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UPlayerDetectAIComponent need UPlayerDetectorComponent"));
		return;
	}

	PlayerDetector->OnNoPlayerDetected.AddDynamic(this, &UPlayerDetectAIComponent::HandleNoPlayerDetected);
	PlayerDetector->OnPlayerFirstDetected.AddDynamic(this, &UPlayerDetectAIComponent::HandlePlayerFirstDetected);
}

void UPlayerDetectAIComponent::HandlePlayerFirstDetected(ACCPaperPlayer* DetectedPlayer)
{
	BlackboardComponent->SetValueAsObject(TEXT("NearbyPlayer"), DetectedPlayer);
}

void UPlayerDetectAIComponent::HandleNoPlayerDetected()
{
	BlackboardComponent->SetValueAsObject(TEXT("NearbyPlayer"), nullptr);
}
