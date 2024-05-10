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


ACCPaperPlayer::ACCPaperPlayer()
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
	GetCharacterMovement()->JumpZVelocity = 15000.f;
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
	FollowCamera->OrthoWidth = 15000.0;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputAction/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	// Flipbook
	PlayerFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PlayerFlipbook"));
	PlayerFlipbook->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Idle/FB_Basic_Idle.FB_Basic_Idle'"));
	if (nullptr != FlipbookRef.Object)
	{
		PlayerFlipbook->SetFlipbook(FlipbookRef.Object);
	}

	// Animation
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookIdleRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Idle/FB_Basic_Idle.FB_Basic_Idle'"));
	if (nullptr != FlipbookIdleRef.Object)
	{
		IdleAnimation = FlipbookIdleRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookMoveRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Move/FB_Basic_Move.FB_Basic_Move'"));
	if (nullptr != FlipbookMoveRef.Object)
	{
		MoveAnimation = FlipbookMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookjumpRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Jump/FB_Basic_Jump.FB_Basic_Jump'"));
	if (nullptr != FlipbookjumpRef.Object)
	{
		JumpAnimation = FlipbookjumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookLandRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Jump/FB_Basic_Land.FB_Basic_Land'"));
	if (nullptr != FlipbookLandRef.Object)
	{
		LandAnimation = FlipbookLandRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookFallingRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Falling/FB_Basic_Falling.FB_Basic_Falling'"));
	if (nullptr != FlipbookFallingRef.Object)
	{
		FallingAnimation = FlipbookFallingRef.Object;
	}

	//Material
	GetSprite()->SetMaterial(0, DefaultSpriteMaterial);
}

void ACCPaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACCPaperPlayer::StartJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACCPaperPlayer::StopJumping);
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
	int32 CurrentFrame = PlayerFlipbook->GetPlaybackPositionInFrames();
	if (CurrentFrame == 3 && ShouldJump == true)
	{
		if (GetCharacterMovement()->IsFalling() == false) Jump();
		ShouldJump = false;
	}
	else if (CurrentFrame > 3)
	{
		SetCurrentState(EPlayerState::Falling);
	}
	else if (CurrentFrame == PlayerFlipbook->GetFlipbook()->GetNumFrames() - 1)
	{
		PlayerFlipbook->Stop();
	}
}

void ACCPaperPlayer::UpdateLand()
{
	int32 CurrentFrame = PlayerFlipbook->GetPlaybackPositionInFrames();
	if (CurrentFrame == PlayerFlipbook->GetFlipbook()->GetNumFrames() - 1)
	{
		SetCurrentState(EPlayerState::Idle);
	}
}

void ACCPaperPlayer::UpdateFalling()
{
	int32 CurrentFrame = PlayerFlipbook->GetPlaybackPositionInFrames();
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		SetCurrentState(EPlayerState::Land);
	}
	else if (CurrentFrame == PlayerFlipbook->GetFlipbook()->GetNumFrames() - 1)
	{
		PlayerFlipbook->Stop();
	}
}

void ACCPaperPlayer::SetCurrentState(EPlayerState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
	case EPlayerState::Idle:
		PlayerFlipbook->Play();
		PlayerFlipbook->SetFlipbook(IdleAnimation);
		break;

	case EPlayerState::Jump:
		PlayerFlipbook->Play();
		PlayerFlipbook->SetFlipbook(JumpAnimation);
		break;

	case EPlayerState::Move:
		PlayerFlipbook->Play();
		PlayerFlipbook->SetFlipbook(MoveAnimation);
		break;

	case EPlayerState::Land:
		PlayerFlipbook->Play();
		PlayerFlipbook->SetFlipbook(LandAnimation);
		break;

	case EPlayerState::Falling:
		PlayerFlipbook->Play();
		PlayerFlipbook->SetFlipbook(FallingAnimation);
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
	if (PlayerFlipbook != nullptr)
	{
		FVector Scale = PlayerFlipbook->GetComponentScale();

		if (InputVector.X < 0)
		{
			Scale.X = FMath::Abs(Scale.X) * -1;
		}
		else if (InputVector.X > 0)
		{
			Scale.X = FMath::Abs(Scale.X);
		}

		PlayerFlipbook->SetWorldScale3D(Scale);
	}
}

void ACCPaperPlayer::StartJump()
{
	if (GetCharacterMovement()->IsFalling() == false)
	{
		ShouldJump = true;
	}
}

void ACCPaperPlayer::Jump()
{
	Super::Jump();

	FVector ForwardVector = PlayerFlipbook->GetRelativeTransform().GetScale3D();
	FVector JumpVelocity = FVector(ForwardVector.X * JumpStrength, 0.0f, JumpHeight);

	LaunchCharacter(JumpVelocity, true, true);
}

void ACCPaperPlayer::StopJumping()
{
	Super::StopJumping();
}
