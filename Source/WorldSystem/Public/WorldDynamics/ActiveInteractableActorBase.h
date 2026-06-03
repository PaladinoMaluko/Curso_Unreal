// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractorEvent/InteractableInterface.h"

#include "ActiveInteractableActorBase.generated.h"

class USceneComponent;

UCLASS(Abstract)
class WORLDSYSTEM_API AActiveInteractableActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActiveInteractableActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* mesh;

public:

#pragma region InterfaceImplement

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InstigatorActor);
	virtual void Interact_Implementation(AActor* InstigatorActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract(AActor* InstigatorActor);
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) override;

#pragma endregion InterfaceImplement

protected:
	bool isActivated = false;

	virtual void Activate(AActor* InstigatorActor);
	virtual void Deactivate(AActor* InstigatorActor);
	virtual bool InteractAvaliable(AActor* InstigatorActor);


};
