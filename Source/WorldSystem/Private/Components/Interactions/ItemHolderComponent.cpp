// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Interactions/ItemHolderComponent.h"

// Sets default values for this component's properties
UItemHolderComponent::UItemHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UItemHolderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemHolderComponent::InitializeItemComponent(USceneComponent* attachComp, FName socketName) {
	AttachComponent = attachComp;
	AttachSocketName = socketName;
}

void UItemHolderComponent::SetHeldItem(AActor* item)
{
	CurrentHeldItem = item;
	hasHeld = CurrentHeldItem != nullptr;
}

AActor* UItemHolderComponent::GetHeldItem() const {
	return CurrentHeldItem;
}

bool UItemHolderComponent::HasHeldItem() const {
	return hasHeld;
}
