// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RotationComponent.h"

// Sets default values for this component's properties
URotationComponent::URotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotationComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
	if (autoStart && !RotatorRate.Euler().IsZero())
		StartRotation();

}

// Called every frame
void URotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Rotation(DeltaTime);
}

void URotationComponent::StartRotation()
{
	normalizedAxis = FVector(RotatorRate.Pitch, RotatorRate.Yaw, RotatorRate.Roll);
	normalizedAxis = normalizedAxis.GetSafeNormal();
	SetComponentTickEnabled(true);
}

void URotationComponent::StopRotation()
{
	SetComponentTickEnabled(false);
}

void URotationComponent::Rotation(float DeltaTime)
{
	FRotator targetRotator = RotatorRate;
	if (revolutionTime > 0)
	{
		const float degreesPerSecond = 360.f / revolutionTime;
		float variation = degreesPerSecond * DeltaTime;

		FRotator deltaRot = FRotator(normalizedAxis.X * variation, normalizedAxis.Y * variation, normalizedAxis.Z * variation);
		GetOwner()->AddActorLocalRotation(deltaRot);
	}
	else
		GetOwner()->AddActorLocalRotation(RotatorRate * DeltaTime);
}