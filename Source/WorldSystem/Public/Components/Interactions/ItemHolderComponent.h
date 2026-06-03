// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemHolderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSYSTEM_API UItemHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void InitializeItemComponent(USceneComponent* attachComp, FName socketName);
	void SetHeldItem(AActor* item);

	FORCEINLINE USceneComponent* GetAttachComponent() const { return AttachComponent; };
	FORCEINLINE FName GetAttachSocketName() const { return AttachSocketName; };
	AActor* GetHeldItem() const;
	bool HasHeldItem() const;

protected:
	UPROPERTY()
	bool hasHeld = false;
	UPROPERTY()
	AActor* CurrentHeldItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Holder")
	USceneComponent* AttachComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Holder")
	FName AttachSocketName;
};

