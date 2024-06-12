// Fill out your copyright notice in the Description page of Project Settings.


#include "CCMapObject.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Character/Player/CCPaperPlayer.h"

// Sets default values
ACCMapObject::ACCMapObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//construct
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Body = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Body"));
	RootComponent = Box;
	IsOverlap = false;
	IsOccupyed = false;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> BodyMeshRef(TEXT("/Game/Sprite/Background/Tree_Sprite.Tree_Sprite"));
	if (BodyMeshRef.Object)
	{
		Body->SetSprite(BodyMeshRef.Object);
	}

	//papersprite
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	Body->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	//box
	Box->InitBoxExtent(FVector(500.0f, 1000.0f, 1000.0f));

	//Overlap delegate
	//Box->OnComponentBeginOverlap.AddDynamic(this, &ACCMapObject::BeginOverlap);
	//Box->OnComponentEndOverlap.AddDynamic(this, &ACCMapObject::EndOverlap);

}


// Called when the game starts or when spawned
void ACCMapObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCMapObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCMapObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ACCPaperPlayer* Player = Cast<ACCPaperPlayer>(OtherActor);
	if (Player)
	{
		PlayerWho(Player);
	}
}

void ACCMapObject::PlayerWho(AActor *Actor)
{
	ACCPaperPlayer* MyCharacter = Cast<ACCPaperPlayer>(Actor);
	if (MyCharacter)
	{
		AController* Controller = MyCharacter->GetController();
		if (Controller)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			if (PlayerController)
			{
				ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
				if (LocalPlayer)
				{
					int32 PlayerIndex = LocalPlayer->GetControllerId();
					UE_LOG(LogTemp, Warning, TEXT("Player Index: %d"), PlayerIndex);
				}
			}
		}
	}
}


//Sprite Change Function
void ACCMapObject::PressSprite()
{

	UPaperSprite* NewSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Sprite/Background/ITree_Sprite.ITree_Sprite'"));

	if (NewSprite)
	{
		Body->SetSprite(NewSprite);
	}
}

void ACCMapObject::ChangeSprite()
{

	UPaperSprite* NewSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Sprite/Background/OTree_Sprite.OTree_Sprite'"));

	if (NewSprite)
	{
		Body->SetSprite(NewSprite);
	}
}

void ACCMapObject::OverlapBeginSprite()
{

	UPaperSprite* OBSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Sprite/Background/CTree_Sprite.CTree_Sprite'"));

	if (OBSprite)
	{
		Body->SetSprite(OBSprite);
	}
}

void ACCMapObject::OverlapEndSprite()
{
	UPaperSprite* OESprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Sprite/Background/Tree_Sprite.Tree_Sprite'"));

	if (OESprite)
	{
		Body->SetSprite(OESprite);
	}
}



