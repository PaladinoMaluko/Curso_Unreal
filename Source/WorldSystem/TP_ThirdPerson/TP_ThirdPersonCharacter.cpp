// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine.h"
#include "Components/Interactions/InteractionDetectorComponent.h"
#include "Components/Interactions/ItemHolderComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaminaComponent.h"
#include "UI/StaminaUserWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	sphereInteractableCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Interactable Component"));
	sphereInteractableCollider->SetupAttachment(RootComponent);

	InteractableDetectorComponent = CreateDefaultSubobject<UInteractionDetectorComponent>(TEXT("Interaction Detector Component"));
	itemHolderComponent = CreateDefaultSubobject<UItemHolderComponent>(TEXT("Item Holder Component"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(sphereInteractableCollider);

	sphereInteractableCollider->OnComponentBeginOverlap.AddDynamic(this, &ATP_ThirdPersonCharacter::OnSphereColliderOverlapBegin);
	sphereInteractableCollider->OnComponentEndOverlap.AddDynamic(this, &ATP_ThirdPersonCharacter::OnSphereColliderOverlapEnd);

	if (itemHolderComponent)
		itemHolderComponent->InitializeItemComponent(GetMesh(), ItemAttachSocketName);

	if (StaminaWidgetClass) {
		StaminaWidget = CreateWidget<UStaminaUserWidget>(GetWorld(), StaminaWidgetClass);
		if (StaminaWidget) {
			StaminaWidget->AddToViewport();
			StaminaComponent->OnStaminaChanged.BindDynamic(StaminaWidget, &UStaminaUserWidget::UpdateStaminaBar);
		}
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Look);
		
		//Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::OnInteractionPressed);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this, &ATP_ThirdPersonCharacter::OnInteractionReleased);

		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATP_ThirdPersonCharacter::OnSprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATP_ThirdPersonCharacter::OnSprintPressed);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_ThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (isSprintButtonPressed && ForwardDirection.Length() > 0) {
			StaminaComponent->TrySprint();
		}
	}
}

void ATP_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATP_ThirdPersonCharacter::OnInteractionPressed(const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "OnInteractionPressed");
	}
	OnInteractionButtonAction(true);
}

void ATP_ThirdPersonCharacter::OnInteractionReleased(const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnInteractionReleased");
	}
	OnInteractionButtonAction(false);
}

void ATP_ThirdPersonCharacter::OnInteractionButtonAction(bool isPressed)
{
	InteractableDetectorComponent->OnInteractionButtonAction(isPressed);
}

void ATP_ThirdPersonCharacter::OnSprintPressed(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	isSprintButtonPressed = isPressed;
	if (GEngine)
	{
		if (isPressed)
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnSprintPressed");
		else
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Not OnSprintPressed");
	}
	if (!isPressed) {
		StaminaComponent->StopSprinting();
	}
}

void ATP_ThirdPersonCharacter::OnSphereColliderOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InteractableDetectorComponent)
		InteractableDetectorComponent->OnInteractionBegin(OtherActor);
}

void ATP_ThirdPersonCharacter::OnSphereColliderOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractableDetectorComponent)
		InteractableDetectorComponent->OnInteractionEnd(OtherActor);
}
