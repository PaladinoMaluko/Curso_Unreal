// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

UCLASS()
class WORLDSYSTEM_API APlatformBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PlatformMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMotionComponent* MotionComponent;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector targetLocationEdit;
};
