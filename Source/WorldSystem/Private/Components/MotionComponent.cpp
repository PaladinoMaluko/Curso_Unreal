// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MotionComponent.h"
#include "Math/UnrealMathUtility.h"

//debug
#include "DrawDebugHelpers.h"
#include "Engine.h"

// Sets default values for this component's properties
UMotionComponent::UMotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//exemple estar after some delay
	/*FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AMyActor::MyFunctionWithParams, MyParam1, MyParam2);
	GetWorldTimerManager().SetTimer(MyTimerHandle, TimerDel, delayToStart, false);*/

	if (GetOwner() != nullptr)
	{
		initialLocation = GetOwner()->GetActorLocation();
	}
	SetComponentTickEnabled(autoStart);
}


// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Motion(DeltaTime);
}

void UMotionComponent::StartMovement()
{
	//start movement 
	cyclesCount = 1;
	if (GetOwner()->GetActorLocation().Equals(initialLocation))
		isForwardDir = true;
	//convert target location to world and back to relative location 
	SetComponentTickEnabled(true);
}
void UMotionComponent::StopMovement()
{
	SetComponentTickEnabled(false);

	if (shoulBackOnStop && !GetOwner()->GetActorLocation().Equals(initialLocation))
		BackToInitialPositionMovement();
}

void UMotionComponent::SetTargetOffset(const FVector newTargetOffset)
{
	targetOffset = newTargetOffset;
}

void UMotionComponent::BackToInitialPositionMovement()
{
	isForwardDir = false;
	stopOnNextCycle = true;
	StartMovement();
}

void UMotionComponent::Motion(float DeltaTime)
{
	if (!GetOwner())
		return;

	FVector initial = isForwardDir ? initialLocation : initialLocation + targetOffset;
	FVector target = isForwardDir ? initialLocation + targetOffset : initialLocation;
	FVector currentLocation = GetOwner()->GetActorLocation();

	const float speed = targetOffset.Length() / period;
	FVector newLocation = FMath::VInterpConstantTo(currentLocation, target, DeltaTime, speed);

	////exemple debug on screen and console
	//if (GEngine)
	//{
	//	const float timeToDisplay = 5;
	//	FString text_to_print = "Teste debug";
	//	//int
	//	//FString::FromInt(value);
	//	// float
	//	//FString::Printf(TEXT("%.*f"), precision, value);
	//	// vector
	//	//FString::Printf(TEXT("X: %.*f, Y: %.*f, Z: %.*f"),precision, vector.X, precision, vector.Y, precision, vector.Z);
	//	GEngine->AddOnScreenDebugMessage(-1, timeToDisplay, FColor::Red, text_to_print);
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *text_to_print);
	//}

	DrawDebugSphere(GetWorld(),
		target,				// Center
		30.0f,              // Radius
		12,                 // Segments
		FColor::Red,        // Color
		false,              // Persistent?
		-1.0f,              // Lifetime (if persistent)
		0,                  // Depth Priority
		2.0f                // Thickness
	);

	GetOwner()->SetActorLocation(newLocation);

	if (newLocation.Equals(target))
	{
		if (maxCycles > 0 && cyclesCount < maxCycles)
			cyclesCount++;
		else if (maxCycles > 0)
		{
			SetComponentTickEnabled(false);
			return;
		}

		if (isPingPongMovement)
			isForwardDir = !isForwardDir;
		else
			GetOwner()->SetActorLocation(initialLocation);

		if (stopOnNextCycle)
		{
			isForwardDir = true;
			SetComponentTickEnabled(false);
		}
	}
}

