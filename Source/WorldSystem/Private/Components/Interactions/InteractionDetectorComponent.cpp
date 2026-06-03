// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Interactions/InteractionDetectorComponent.h"
#include "InteractorEvent/InteractableInterface.h"
#include <Components/Interactions/ItemHolderComponent.h>

// Sets default values for this component's properties
UInteractionDetectorComponent::UInteractionDetectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void UInteractionDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	OverlapActors.Empty();
	holderComponent = GetOwner()->FindComponentByClass<UItemHolderComponent>();

	if (InfoTextActor)
	{
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();

		SpawnedTextActor = GetWorld()->SpawnActor<AActor>(InfoTextActor, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		SetTextInfoVisibiity(false);
	}
}

void UInteractionDetectorComponent::OnInteractionBegin(AActor* OtherActor)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		OverlapActors.AddUnique(OtherActor);

	Interaction();
}

void UInteractionDetectorComponent::OnInteractionEnd(AActor* OtherActor)
{
	if (OtherActor && OtherActor->Implements<UInteractableInterface>())
		OverlapActors.Remove(OtherActor);

	Interaction();
}

void UInteractionDetectorComponent::OnInteractionButtonAction(bool isPressed)
{
	isActioButtonPressed = isPressed;
	if (isActioButtonPressed)
	{
		Interaction();

		AActor* interactableComponent = NearInteractingActor;
		bool isItemType = false;
		if (holderComponent && holderComponent->HasHeldItem())
		{
			interactableComponent = holderComponent->GetHeldItem();
			isItemType = true;
		}

		if (!interactableComponent)
			return;

		if (isItemType || IInteractableInterface::Execute_CanInteract(interactableComponent, this->GetOwner()))
		{
			IInteractableInterface::Execute_SetInteractionFeedback(interactableComponent, false);
			IInteractableInterface::Execute_Interact(interactableComponent, this->GetOwner());
		}

	}
}

void UInteractionDetectorComponent::Interaction()
{
	//get near interacting actor
	AActor* nearInteracting = GetNearInteractingActor();
	bool canInteract = nearInteracting != nullptr;

	if (NearInteractingActor)
	{
		IInteractableInterface::Execute_SetInteractionFeedback(NearInteractingActor, false);
		SetTextInfoVisibiity(false);
	}

	if (canInteract)
	{
		//add feedback
		IInteractableInterface::Execute_SetInteractionFeedback(nearInteracting, true);
		if (USceneComponent* sceneTextLocation = IInteractableInterface::Execute_GetSceneInfoComponent(nearInteracting))
		{
			UpdateTextInfoLocation(sceneTextLocation);
			SetTextInfoVisibiity(true);
		}

		NearInteractingActor = nearInteracting;
		return;
	}
	NearInteractingActor = nullptr;
}

AActor* UInteractionDetectorComponent::GetNearInteractingActor()
{
	AActor* nearInteractingActor = nullptr;
	float nearDistance = FLT_MAX;
	const FVector ownerLocation = GetOwner()->GetActorLocation();

	for (int32 i = 0; i < OverlapActors.Num(); i++)
	{
		AActor* actor = OverlapActors[i];
		UE_LOG(LogTemp, Display, TEXT("Actor name: %s"),*actor->GetName());
		//if (actor && actor->Implements<UInteractableInterface>())
		if (actor && actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			const float distance = FVector::DistSquared(ownerLocation, actor->GetActorLocation());
			if (distance < nearDistance)
			{
				//bool isInteractable = IInteractableInterface::Execute_CanInteract(actor, GetOwner());
				if (!IInteractableInterface::Execute_CanInteract(actor, GetOwner()))
					continue;

				nearDistance = distance;
				nearInteractingActor = actor;
			}
		}
	}
	return nearInteractingActor;
}

void UInteractionDetectorComponent::SetTextInfoVisibiity(bool isVisible)
{
	if (SpawnedTextActor)
		SpawnedTextActor->SetActorHiddenInGame(!isVisible);
}

void UInteractionDetectorComponent::UpdateTextInfoLocation(USceneComponent* sceneInfoLocation)
{
	if (SpawnedTextActor && sceneInfoLocation)
		SpawnedTextActor->SetActorLocation(sceneInfoLocation->GetComponentLocation());
}

