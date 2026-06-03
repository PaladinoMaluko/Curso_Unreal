// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldDynamics/PlatformBase.h"
#include "Components/StaticMeshComponent.h"
#include "../../Public/Components/MotionComponent.h"

#include "Engine.h"

// Sets default values
APlatformBase::APlatformBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Platform Root"));
	RootComponent = Root;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	PlatformMesh->SetupAttachment(Root);

	MotionComponent = CreateDefaultSubobject<UMotionComponent>(TEXT("Motion Component"));
}

// Called when the game starts or when spawned
void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Display, TEXT("AFTER BEGIN PLAY"));
		/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, TEXT("TESTE PRINT TELA"));
	}*/
}

void APlatformBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!targetLocationEdit.Equals(FVector::ZeroVector) && MotionComponent)
	{
		MotionComponent->SetTargetOffset(targetLocationEdit);
	}
}

