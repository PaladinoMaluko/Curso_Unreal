// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnStaminaChanged, float, Percent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSYSTEM_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UPROPERTY()
	FOnStaminaChanged OnStaminaChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SprintSpeed = 1500.0f;

	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DrainRate = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float RegenRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStaminaToSprint = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float RegenDelay = 1.0f;

private:
	float WalkSpeed;
	bool IsSprinting = false;
	float currentStamina;
	float TimeSinceStopped = 0.0f;

public:
	void TrySprint();
	void StopSprinting();

protected:
	void StartSprinting();
	bool CanSprint() const;
	void UpdateStamina(float Amount);
};
