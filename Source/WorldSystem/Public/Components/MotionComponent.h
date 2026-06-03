// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSYSTEM_API UMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMotionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartMovement();
	UFUNCTION(BlueprintCallable)
	void StopMovement();
	UFUNCTION(BlueprintCallable)
	void SetTargetOffset(const FVector newTargetOffset);


protected:
	//seg
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Motion Params")
	float period = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Params")
	FVector targetOffset = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Params")
	bool autoStart = false;

	UPROPERTY(EditAnywhere, Category = "Motion Params")
	bool isPingPongMovement = true;
	// 0 = infinit
	UPROPERTY(EditAnywhere, Category = "Motion Params")
	int32 maxCycles = 0;
	
	UPROPERTY(EditAnywhere, Category = "Motion Params")
	bool shoulBackOnStop = false;

	UFUNCTION(BlueprintCallable)
	void BackToInitialPositionMovement();

private:
	//vars
	FVector initialLocation;
	int32 cyclesCount = 1;
	bool isForwardDir = true;
	bool stopOnNextCycle = false;
	//functions
	void Motion(float DeltaTime);
};
