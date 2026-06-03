// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamics/PassiveInteractable/SlotItemInteractableActor.h"
#include "InteractorEvent/InteractableInterface.h"

ASlotItemInteractableActor::ASlotItemInteractableActor()
{
	SlotAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SlotAttachPoint"));
	SlotAttachPoint->SetupAttachment(Root);
}

void ASlotItemInteractableActor::AttachItem(AActor* actor)
{
	UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(actor->GetRootComponent());
	if (prim)
	{
		prim->SetSimulatePhysics(false);
		prim->SetEnableGravity(false);
	}
	IsSlotFilled = actor->AttachToComponent(SlotAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (IsSlotFilled)
		ItemAttached = actor;
}

void ASlotItemInteractableActor::CallInterface(bool isBeginOverlap, AActor* OtherActor)
{
	if (isBeginOverlap && IsSlotFilled)
		return;

	if (isBeginOverlap)
		AttachItem(OtherActor);
	else if (IsSlotFilled && OtherActor == ItemAttached)
	{
		ItemAttached = nullptr;
		IsSlotFilled = false;
	}
	else if (IsSlotFilled)
		return;

	if (AditionalTag.IsNone() || OtherActor->ActorHasTag(AditionalTag))
		Super::CallInterface(isBeginOverlap, OtherActor);
}

