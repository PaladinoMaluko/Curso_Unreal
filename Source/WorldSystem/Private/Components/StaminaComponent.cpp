// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent) {
		WalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
	}

	checkf(CharacterMovementComponent, TEXT("StaminaComponent requires a CharacterMovementComponent"))

	currentStamina = MaxStamina;
	SetComponentTickEnabled(false);
	float staminaNormalized = currentStamina / MaxStamina;
	OnStaminaChanged.ExecuteIfBound(staminaNormalized);
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsSprinting) {
		UpdateStamina(-DrainRate * DeltaTime);
		if (currentStamina <= 0.0f) {
			StopSprinting();
		}
	}
	else {
		TimeSinceStopped += DeltaTime;
		if (TimeSinceStopped >= RegenDelay) {
			UpdateStamina(RegenRate * DeltaTime);
			if (currentStamina >= MaxStamina) {
				SetComponentTickEnabled(false);
			}
		}
	}

	if (GEngine)
	{
		float staminaNormalized = currentStamina / MaxStamina;
		FString staminaNormStr = FString::SanitizeFloat(staminaNormalized, 3);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, staminaNormStr);
	}
}

void UStaminaComponent::TrySprint()
{
	if (IsSprinting || !CanSprint()) return;

	StartSprinting();
}

void UStaminaComponent::StopSprinting()
{
	IsSprinting = false;
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
	TimeSinceStopped = 0.0f;
}

void UStaminaComponent::StartSprinting()
{
	IsSprinting = true;
	CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
	SetComponentTickEnabled(true);

}

bool UStaminaComponent::CanSprint() const
{
	return currentStamina >= MinStaminaToSprint;
}

void UStaminaComponent::UpdateStamina(float Amount)
{
	currentStamina += Amount;
	currentStamina = FMath::Clamp(currentStamina, 0.0f, MaxStamina);

	float staminaNormalized = currentStamina / MaxStamina;
	OnStaminaChanged.ExecuteIfBound(staminaNormalized);
}



