// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamics/PassiveInteractableActorBase.h"
#include "Components/ShapeComponent.h"
#include "InteractorEvent/InteractableInterface.h"

// Sets default values
APassiveInteractableActorBase::APassiveInteractableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

}
// Called when the game starts or when spawned
void APassiveInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	triggerColliderComponent = FindComponentByClass<UShapeComponent>();
	check(triggerColliderComponent);

	triggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APassiveInteractableActorBase::OnOverlapBegin);
	triggerColliderComponent->OnComponentEndOverlap.AddDynamic(this, &APassiveInteractableActorBase::OnOverlapEnd);
}

void APassiveInteractableActorBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((RequiredTag.IsNone() || OtherActor->ActorHasTag(RequiredTag)))
		CallInterface(true, OtherActor);
}

void APassiveInteractableActorBase::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((RequiredTag.IsNone() || OtherActor->ActorHasTag(RequiredTag)))
		CallInterface(false, OtherActor);
}

void APassiveInteractableActorBase::CallInterface(bool isBeginOverlap, AActor* OtherActor)
{
	for (int32 i = 0; i < interactableActors.Num(); i++)
	{
		if (!interactableActors[i] ||
			!interactableActors[i]->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			continue;

		if (isBeginOverlap)
		{
			if (IInteractableInterface::Execute_CanInteract(interactableActors[i], nullptr))
				IInteractableInterface::Execute_OnEnterInteractableZone(interactableActors[i]);
		}
		else
		{
			IInteractableInterface::Execute_OnLeaveInteractableZone(interactableActors[i]);
		}
	}
}
