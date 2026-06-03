// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxTriggerComponent.h"
#include "Engine.h"
#include "Components/MotionComponent.h"

UBoxTriggerComponent::UBoxTriggerComponent()
{
}

void UBoxTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (movableActor)
	{
		motionComponent =
			movableActor->FindComponentByClass<UMotionComponent>();
	}

	if (!motionComponent)
		return;

	OnComponentBeginOverlap.AddDynamic(this, &UBoxTriggerComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UBoxTriggerComponent::OnOverlapEnd);
}

void UBoxTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "UBoxTriggerComponent::OnOverlapBegin");
	}
	motionComponent->StartMovement();
}

void UBoxTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "UBoxTriggerComponent::OnOverlapEnd");
	}
	if (shouldStopOnEndOverlap)
		motionComponent->StopMovement();
}

