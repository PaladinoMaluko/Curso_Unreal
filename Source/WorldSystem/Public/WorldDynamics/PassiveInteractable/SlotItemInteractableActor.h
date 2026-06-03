// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldDynamics/PassiveInteractableActorBase.h"
#include "SlotItemInteractableActor.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSYSTEM_API ASlotItemInteractableActor : public APassiveInteractableActorBase
{
	GENERATED_BODY()
public:
	ASlotItemInteractableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AditionalTag = FName();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SlotAttachPoint;

	void AttachItem(AActor* actor);

	virtual void CallInterface(bool isBeginOverlap, AActor* OtherActor) override;

	bool IsSlotFilled = false;

	UPROPERTY()
	AActor* ItemAttached = nullptr;
	
};
