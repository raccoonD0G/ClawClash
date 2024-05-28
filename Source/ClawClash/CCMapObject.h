// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCMapObject.generated.h"



UCLASS()
class CLAWCLASH_API ACCMapObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACCMapObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Box;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void PlayerWho(AActor* Actor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sprite Change Function
	void OverlapBeginSprite();
	void OverlapEndSprite();
	void ChangeSprite();
	void PressSprite();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandle")
	bool IsOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandle")
	bool IsOccupyed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UPaperSpriteComponent> Body;


};
