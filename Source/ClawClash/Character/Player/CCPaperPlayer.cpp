// Fill out your copyright notice in the Description page of Project Settings.


#include "CCPaperPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClawClash/Managers/LayerManager/CCLayerManager.h"
#include "ClawClash/Components/HealthComponent.h"


ACCPaperPlayer::ACCPaperPlayer() : Super()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// CharacterMovement
	GetCharacterMovement()->MaxWalkSpeed = 5000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 10000.f;
	GetCharacterMovement()->AirControl = 100.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.f;
	GetCharacterMovement()->GravityScale = 8.0f; 
	GetCharacterMovement()->MaxAcceleration = 10000;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));

	// Capsule
	GetCapsuleComponent()->SetCapsuleRadius(200.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(200.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->BodyInstance.bLockYTranslation = true;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 7000.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(7000.0f, 7000.0f, 0.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	FollowCamera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	FollowCamera->PostProcessSettings.MotionBlurAmount = 0.0f;
	FollowCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	FollowCamera->OrthoWidth = 12000.0;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->Init(MaxHp);


	if (IdleAnimation)
	{
		GetSprite()->SetFlipbook(IdleAnimation);
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.Y = UCCLayerManager::GetPlayerY();
	SetActorLocation(NewLocation);
}

void ACCPaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACCPaperPlayer::ReadyJump);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACCPaperPlayer::Move);
}

void ACCPaperPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

}

void ACCPaperPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EPlayerState::Idle:
		UpdateIdle();
		break;

	case EPlayerState::Jump:
		UpdateJump();
		break;

	case EPlayerState::Move:
		UpdateMove();
		break;

	case EPlayerState::Land:
		UpdateLand();
		break;

	case EPlayerState::Falling:
		UpdateFalling();
		break;

	default:
		break;
	}
}

void ACCPaperPlayer::UpdateIdle()
{
	if (ShouldJump == true)
	{
		SetCurrentState(EPlayerState::Jump);
	}
	else
	{
		float Speed = GetVelocity().Size();
		if (GetCharacterMovement()->IsMovingOnGround() && Speed > PlayerIdleThreshold)
		{
			SetCurrentState(EPlayerState::Move);
		}

		if (GetCharacterMovement()->IsFalling())
		{
			SetCurrentState(EPlayerState::Falling);
		}
	}
}

void ACCPaperPlayer::UpdateMove()
{
	if (ShouldJump == true)
	{
		SetCurrentState(EPlayerState::Jump);
	}
	else
	{
		float Speed = GetVelocity().Size();
		if (GetCharacterMovement()->IsMovingOnGround() && Speed <= PlayerIdleThreshold)
		{
			SetCurrentState(EPlayerState::Idle);
		}

		if (GetCharacterMovement()->IsFalling())
		{
			SetCurrentState(EPlayerState::Falling);
		}
	}
}

void ACCPaperPlayer::UpdateJump()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (CurrentFrame == 3 && ShouldJump == true)
	{
		if (GetCharacterMovement()->IsFalling() == false) Jump();
		ShouldJump = false;
	}
	else if (CurrentFrame > 3)
	{
		SetCurrentState(EPlayerState::Falling);
	}
	else if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		GetSprite()->Stop();
	}
}

void ACCPaperPlayer::UpdateLand()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		SetCurrentState(EPlayerState::Idle);
	}
}

void ACCPaperPlayer::UpdateFalling()
{
	int32 CurrentFrame = GetSprite()->GetPlaybackPositionInFrames();
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		SetCurrentState(EPlayerState::Land);
	}
	else if (CurrentFrame == GetSprite()->GetFlipbook()->GetNumFrames() - 1)
	{
		GetSprite()->Stop();
	}
}

void ACCPaperPlayer::SetCurrentState(EPlayerState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
	case EPlayerState::Idle:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(IdleAnimation);
		break;

	case EPlayerState::Jump:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(JumpAnimation);
		break;

	case EPlayerState::Move:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(MoveAnimation);
		break;

	case EPlayerState::Land:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(LandAnimation);
		break;

	case EPlayerState::Falling:
		GetSprite()->Play();
		GetSprite()->SetFlipbook(FallingAnimation);
		break;

	default:
		break;
	}
}

void ACCPaperPlayer::Move(const FInputActionValue& Value)
{
	if (CurrentState == EPlayerState::Jump || CurrentState == EPlayerState::Jump) return;

	FVector2D InputVector = Value.Get<FVector2D>();
	AddMovementInput(FVector(1.f, 0.f, 0.f), InputVector.X * 1.0f);
	
	FVector Scale = GetSprite()->GetComponentScale();

	if (InputVector.X < 0)
	{
		Scale.X = FMath::Abs(Scale.X) * -1;
	}
	else if (InputVector.X > 0)
	{
		Scale.X = FMath::Abs(Scale.X);
	}

	GetSprite()->SetWorldScale3D(Scale);
}

void ACCPaperPlayer::ReadyJump()
{
	if (GetCharacterMovement()->IsFalling() == false)
	{
		ShouldJump = true;
	}
}

void ACCPaperPlayer::Jump()
{
	Super::Jump();

	FVector ForwardVector = GetSprite()->GetRelativeTransform().GetScale3D();
	FVector JumpVelocity = FVector(ForwardVector.X * JumpStrength, 0.0f, JumpHeight);

	LaunchCharacter(JumpVelocity, true, true);
}

float ACCPaperPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCause)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCause);
	return 0.0f;
}
