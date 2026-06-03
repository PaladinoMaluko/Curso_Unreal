// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemHolderComponent.h"
#include "InteractionDetectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSYSTEM_API UInteractionDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionDetectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	void OnInteractionBegin(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnInteractionEnd(AActor* OtherActor);

	FORCEINLINE void SetIsActionButtonPressed(bool bIsPressed)
	{isActioButtonPressed = bIsPressed;}

	void OnInteractionButtonAction(bool isPressed);

protected:
	UItemHolderComponent* holderComponent;
	bool isActioButtonPressed = false;

	TArray<AActor*> OverlapActors;
	AActor* NearInteractingActor;

	void Interaction();
	AActor* GetNearInteractingActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> InfoTextActor;

protected:
	UPROPERTY()
	AActor* SpawnedTextActor;

	void SetTextInfoVisibiity(bool isVisible);

	void UpdateTextInfoLocation(USceneComponent* sceneInfoLocation);

};
