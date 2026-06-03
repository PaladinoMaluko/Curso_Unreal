// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamics/ActiveInteractable/Item.h"
#include "Engine.h"
#include <Components/Interactions/ItemHolderComponent.h>

void AItem::BeginPlay()
{
	Super::BeginPlay();
	if (!primitiveComp)
	{
		primitiveComp = FindComponentByClass<UPrimitiveComponent>();
		isPhysicsSimulate = primitiveComp->IsSimulatingPhysics();
		isGravityEnable = primitiveComp->IsGravityEnabled();
	}
}

void AItem::Activate(AActor* InstigatorActor) {
	Super::Activate(InstigatorActor);
	USceneComponent* attachTo = nullptr;
	FName socketName = NAME_None;
	UItemHolderComponent* itemHolder = InstigatorActor->FindComponentByClass<UItemHolderComponent>();


	if (itemHolder)
	{
		attachTo = itemHolder->GetAttachComponent();
		socketName = itemHolder->GetAttachSocketName();
	}
	else
	{
		attachTo = InstigatorActor->GetRootComponent();
	}

	if (!attachTo)
		return;

	SetActivateSimulatePhysics(false, false);
	SetActorEnableCollision(false);

	if (AttachToComponent(
		attachTo,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		socketName))
	{
		isAttached = true;

		if (itemHolder)
		{
			holder = InstigatorActor;
			isAttached = true;
			itemHolder->SetHeldItem(this);
		}
	}

}

void AItem::Deactivate(AActor* InstigatorActor)
{
	Super::Deactivate(InstigatorActor);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	holder = nullptr;
	isAttached = false;
	if (UItemHolderComponent* itemHolder = InstigatorActor->FindComponentByClass<UItemHolderComponent>())
	{
		itemHolder->SetHeldItem(nullptr);
	}

	SetActivateSimulatePhysics(isPhysicsSimulate, isGravityEnable);
	SetActorEnableCollision(true);
}

bool AItem::InteractAvaliable(AActor* InstigatorActor)
{
	if (UItemHolderComponent* itemHolder = InstigatorActor->FindComponentByClass<UItemHolderComponent>())
		return !isAttached && !itemHolder->HasHeldItem();

	return false;
}

void AItem::SetActivateSimulatePhysics(bool activate, bool isGravityActive)
{
	if (primitiveComp)
	{
		primitiveComp->SetSimulatePhysics(activate);
		primitiveComp->SetEnableGravity(isGravityActive);
	}
}