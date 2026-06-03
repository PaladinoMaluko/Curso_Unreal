// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PassiveInteractableActorBase.generated.h"

UCLASS()
class WORLDSYSTEM_API APassiveInteractableActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APassiveInteractableActorBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> interactableActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RequiredTag = FName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UShapeComponent* triggerColliderComponent;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void CallInterface(bool isBeginOverlap, AActor* OtherActor);


};
