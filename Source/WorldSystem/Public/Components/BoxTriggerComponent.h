// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "BoxTriggerComponent.generated.h"
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WORLDSYSTEM_API UBoxTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UBoxTriggerComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* movableActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool shouldStopOnEndOverlap = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** called when something enters the sphere component */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	class UMotionComponent* motionComponent;

};
