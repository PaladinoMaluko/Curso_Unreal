
// Fill out your copyright notice in the Description page of Project Settings.
//Pragma once is a preprocessor directive 
// used in header files to ensure that 
// the file is included only once during 
// a single compilation. 
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WORLDSYSTEM_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. 
	// This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	void OnEnterInteractableZone();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	void OnLeaveInteractableZone();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	bool CanInteract(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	void Interact(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	void SetInteractionFeedback(bool activate);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable Event")
	class USceneComponent* GetSceneInfoComponent();

};
