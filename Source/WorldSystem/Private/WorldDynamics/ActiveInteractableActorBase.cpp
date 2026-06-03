// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamics/ActiveInteractableActorBase.h"

// Sets default values
AActiveInteractableActorBase::AActiveInteractableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AActiveInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AActiveInteractableActorBase::CanInteract_Implementation(AActor* InstigatorActor)
{
	return InteractAvaliable(InstigatorActor);
}

void AActiveInteractableActorBase::Interact_Implementation(AActor* InstigatorActor)
{
	//toggle
	if (isActivated)
		Deactivate(InstigatorActor);
	else
		Activate(InstigatorActor);
}

bool AActiveInteractableActorBase::InteractAvaliable(AActor* InstigatorActor)
{
	return true;
}

void AActiveInteractableActorBase::Activate(AActor* InstigatorActor)
{
	isActivated = true;
}

void AActiveInteractableActorBase::Deactivate(AActor* InstigatorActor)
{
	isActivated = false;
}