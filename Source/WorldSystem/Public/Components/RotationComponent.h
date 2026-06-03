// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDSYSTEM_API URotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//whe its zero,  follow the rorator rate 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float revolutionTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotatorRate;

	UPROPERTY(EditAnywhere)
	bool autoStart = true;

	UFUNCTION(BlueprintCallable)
	void StartRotation();
	UFUNCTION(BlueprintCallable)
	void StopRotation();

private:
	void Rotation(float DeltaTime);
	FVector normalizedAxis;
};
