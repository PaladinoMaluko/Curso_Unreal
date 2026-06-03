// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldDynamics/ActiveInteractableActorBase.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSYSTEM_API AItem final : public AActiveInteractableActorBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	bool isAttached = false;
	AActor* holder = nullptr;
	virtual void Activate(AActor* InstigatorActor) override;
	virtual void Deactivate(AActor* InstigatorActor) override;
	virtual bool InteractAvaliable(AActor* InstigatorActor) override;
	void SetActivateSimulatePhysics(bool activate, bool isGravityActive);
	bool isGravityEnable = false;
	bool isPhysicsSimulate = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrimitiveComponent* primitiveComp;
};

