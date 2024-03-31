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
	GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 2000.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 3.0f; 
	GetCharacterMovement()->MaxAcceleration = 3000;

	// Capsule
	GetCapsuleComponent()->SetCapsuleRadius(200.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(200.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(400.0f, 400.0f, 0.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	FollowCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	FollowCamera->OrthoWidth = 5000.0;

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

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookMoveRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Runnig/FB_Basic_Running.FB_Basic_Running'"));
	if (nullptr != FlipbookMoveRef.Object)
	{
		MoveAnimation = FlipbookMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookjumpRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Sprite/Player/Basic_Runnig/FB_Basic_Running.FB_Basic_Running'"));
	if (nullptr != FlipbookjumpRef.Object)
	{
		JumpAnimation = FlipbookjumpRef.Object;
	}
}

void ACCPaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACCPaperPlayer::Jump);
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

	float Speed = GetVelocity().Size();

	if (GetCharacterMovement()->IsFalling())
	{
		SetCurrentState(FGameDefinitions::EPlayerState::Jump);
	}
	else if (GetCharacterMovement()->IsMovingOnGround() && Speed > PlayerIdleThreshold)
	{
		SetCurrentState(FGameDefinitions::EPlayerState::Move);
	}
	else
	{
		SetCurrentState(FGameDefinitions::EPlayerState::Idle);
	}

	switch (CurrentState)
	{
	case FGameDefinitions::EPlayerState::Idle:
		UpdateIdle();
		break;

	case FGameDefinitions::EPlayerState::Jump:
		UpdateJump();
		break;

	case FGameDefinitions::EPlayerState::Move:
		UpdateMove();
		break;

	default:
		break;
	}
}

void ACCPaperPlayer::UpdateIdle()
{

}

void ACCPaperPlayer::UpdateMove()
{

}

void ACCPaperPlayer::UpdateJump()
{

}

void ACCPaperPlayer::SetCurrentState(FGameDefinitions::EPlayerState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
	case FGameDefinitions::EPlayerState::Idle:
		PlayerFlipbook->SetFlipbook(IdleAnimation);
		break;

	case FGameDefinitions::EPlayerState::Jump:
		PlayerFlipbook->SetFlipbook(JumpAnimation);
		break;

	case FGameDefinitions::EPlayerState::Move:
		PlayerFlipbook->SetFlipbook(MoveAnimation);
		break;

	default:
		break;
	}
}

void ACCPaperPlayer::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	AddMovementInput(FVector(1.f, 0.f, 0.f), InputVector.X * 1000.0f);
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

void ACCPaperPlayer::Jump()
{
	Super::Jump();
}

void ACCPaperPlayer::StopJumping()
{
	Super::StopJumping();
}
